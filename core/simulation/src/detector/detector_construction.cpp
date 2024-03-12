//
// Created by Yulei on 2024/3/9.
//

#include "detector/detector_construction.h"
#include "detector/sensitive_detector.h"

#include <ranges>
#include <cmath>

#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4VisAttributes.hh>
#include <G4PVParameterised.hh>

#include <Math/RotationX.h>
#include <Math/RotationY.h>
#include <Math/RotationZ.h>
#include <Math/Rotation3D.h>
#include <Math/Vector3D.h>

using RotationX = ROOT::Math::RotationX;
using RotationY = ROOT::Math::RotationY;
using RotationZ = ROOT::Math::RotationZ;
using Rotation3D = ROOT::Math::Rotation3D;
using XYZVector = ROOT::Math::XYZVector;

namespace SimREX::Simulation {
    G4ThreadLocal magnetic_field* detector_construction::_magneticField = nullptr;
    G4ThreadLocal G4FieldManager* detector_construction::_fieldMgr = nullptr;


    detector_construction::detector_construction() {
        _logger = GEM::LoggerManager::getInstance()->createLogger("Detector Construction");
        _logger->info("constructor called.");
    }

    detector_construction::~detector_construction() {
        _logger->info("destructor called.");

        for (auto& [name, sub_layer_placement] : _sub_layer_placement)
            delete sub_layer_placement;
    }

    G4VPhysicalVolume* detector_construction::Construct() {
        _logger->info("construct called.");

        const bool check_overlap = false;

        // geometries
        buildWorldVolume();

        buildTarget();

        buildTracker(check_overlap);

        buildCalorimeter();

        // update world size and return
        const auto world_box = dynamic_cast<G4Box*>(_world_LV->GetSolid());
        world_box->SetXHalfLength(_world_size[0] / 2.0);
        world_box->SetYHalfLength(_world_size[1] / 2.0);
        world_box->SetZHalfLength(_world_size[2] / 2.0);
        G4RunManager::GetRunManager()->GeometryHasBeenModified();

        return _world_PV;
    }

    void detector_construction::ConstructSDandField() {
#ifdef DEBUG
        _logger->info("construct SD and field called.");
#endif

        // Sensitive Detector
        for (const auto& [_, _placement] : _sub_layer_placement) {
            for (const auto& [_name, _lv] : _placement->get_tracker().strips_LV) {
                const auto sd = new sensitive_detector(_name, db::det_type::tracker, G4Threading::G4GetThreadId());
                _lv->SetSensitiveDetector(sd);
            }
        }

        // magnetic field
        _magneticField = new magnetic_field();
        _fieldMgr = new G4FieldManager();
        _fieldMgr->SetDetectorField(_magneticField);
        _fieldMgr->CreateChordFinder(_magneticField);
        G4bool forceToAllDaughters = true;
        //        fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);
    }

    void detector_construction::buildWorldVolume(bool check_overlap) {
        // experimental hall (world volume)

        const auto material = db::Instance()->get<std::string>("world/material");
        G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial(material);

        const auto size = db::Instance()->get<std::vector<double>>("world/size");
        _world_size = {size[0], size[1], size[2]};

        const auto world_solid = new G4Box(
            "world_box",
            _world_size[0] / 2.0, _world_size[1] / 2.0, _world_size[2] / 2.0
        );
        _world_LV = new G4LogicalVolume(world_solid, world_mat, "world_LV");
        _world_PV = new G4PVPlacement(
            nullptr, G4ThreeVector(), _world_LV, "world", nullptr, false, 0, check_overlap);
    }

    void detector_construction::buildTarget(bool check_overlap) {}

    void detector_construction::buildTracker(bool check_overlap) {
        const auto build_list = db::Instance()->get<std::vector<std::string>>("build-list");

        /*
         * Tracker Region
         *   - layer
         *     - sub-layer
         *       - strip
         */

        // Build Tracker Region Mannually
        for (
            auto tracker_regions = db::Instance()->get<std::vector<tracker_region>>("tracker-like");
            auto& [name, material, position, layers, size] : tracker_regions
        ) {
            if (std::ranges::find(build_list, name) == build_list.end()) {
                _logger->warn("tracker region [{}] not in the building list, IGNORE", name);
                continue;
            }

            _logger->info("building tracker region [{}]", name);

            auto pre_computing_size = [name, layers, this]() {
                std::vector<double> max_size = {0, 0, 0};

                // For each daughter, compute the max size
                for (const auto& layer : layers) {
                    // Step 1: Find the index of the max of abs of the position (mother need to be square)
                    auto find_max_index = [layer, this](int axis) {
                        assert(layer.position.size() == layer.size.size());

                        const double x = layer.size[0] / 2.0;
                        const double y = layer.size[1] / 2.0;
                        const double z = layer.size[2] / 2.0 + layer.offset + 2 * _eps;

                        double max_size_axis;
                        if (!layer.rotation.empty()) {
                            // default geometry is box, calculate the 8 vertices
                            const std::vector<XYZVector> vertices = {
                                {x, y, z},
                                {x, -y, z},
                                {x, y, -z},
                                {x, -y, -z},
                                {-x, y, z},
                                {-x, -y, z},
                                {-x, y, -z},
                                {-x, -y, -z}
                            };

                            double max_len = 0;
                            for (const auto& rot : layer.rotation) {
                                // define rotation
                                const auto rot_x = RotationX(rot[0]);
                                const auto rot_y = RotationY(rot[1]);
                                const auto rot_z = RotationZ(rot[2]);

                                for (const auto& vertex : vertices) {
                                    XYZVector new_vertex = rot_z(rot_y(rot_x(vertex)));
                                    if (axis == 0) max_len = std::max(max_len, std::abs(new_vertex.x()));
                                    if (axis == 1) max_len = std::max(max_len, std::abs(new_vertex.y()));
                                    if (axis == 2) max_len = std::max(max_len, std::abs(new_vertex.z()));
                                }
                            }
                            max_size_axis = max_len;
                        }
                        else {
                            max_size_axis = layer.size[axis] / 2.0;
                        }
                        // return the maximum position+size (convert half length to size) for each daughter
                        return (max_size_axis + std::abs(layer.position[axis]) + layer.offset) * 2 + 2 * _eps;
                    };

                    max_size = {
                        find_max_index(0),
                        find_max_index(1),
                        find_max_index(2)
                    };

                    max_size = {
                        std::max(max_size[0], find_max_index(0)),
                        std::max(max_size[1], find_max_index(1)),
                        std::max(max_size[2], find_max_index(2))
                    };
                }
                return max_size;
            };

            size = pre_computing_size();
            db::Instance()->set(std::format("tracker-like/{}/size", name), size);

            // Step 1: Build Region
            const auto tracker_region_solid = new G4Box(
                std::format("{}_box", name), size[0] / 2., size[1] / 2., size[2] / 2.);
            const auto tracker_region_mat = G4NistManager::Instance()->FindOrBuildMaterial(material);
            const auto tracker_region_LV = new G4LogicalVolume(
                tracker_region_solid, tracker_region_mat, std::format("{}_LV", name));

            new G4PVPlacement(
                nullptr, G4ThreeVector(position[0], position[1], position[2]),
                tracker_region_LV, name, _world_LV, false, 0, check_overlap
            );

            // Step 2: Build Daughter Layers
            for (const auto& layer : layers) {
                const auto trk_solid = new G4Box(
                    std::format("{}_box", layer.name),
                    (size[0] - _eps) / 2., (size[1] - _eps) / 2., layer.size[2] / 2.0 + layer.offset + _eps
                );
                const auto trk_mat = G4NistManager::Instance()->FindOrBuildMaterial(material);
                const auto trk_LV = new G4LogicalVolume(
                    trk_solid, trk_mat, std::format("{}_LV", layer.name));

                new G4PVPlacement(
                    nullptr, G4ThreeVector(layer.position[0], layer.position[1], layer.position[2]),
                    trk_LV, layer.name, tracker_region_LV, false, 0, check_overlap
                );

                _logger->info(
                    "Layer {}, position [{}, {}, {}], size [{}, {}, {}], build size [{}, {}, {}]",
                    layer.name, layer.position[0], layer.position[1], layer.position[2],
                    layer.size[0], layer.size[1], layer.size[2],
                    (size[0] - _eps), (size[1] - _eps), layer.size[2] + 2 * layer.offset + _eps
                );

                // For each sub-layer in layer, place the detailed geometry
                auto sub_layer_name = std::format("{}_layer", layer.name);
                _sub_layer_placement[sub_layer_name] = new matrix_parameterization(
                    layer.material, layer.name,
                    {layer.size[0], layer.size[1], layer.size[2]},
                    {layer.position[0], layer.position[1], layer.position[2]},
                    layer.rotation,
                    layer.offset, layer.strip_per_layer,
                    trk_LV,
                    check_overlap
                );

                const auto _sub_layer = _sub_layer_placement[sub_layer_name]->get_tracker();
                for (const auto& [_sub_layer_name, sub_layer_LV] : _sub_layer.sub_layers_LV) {
                    new G4PVParameterised(
                        sub_layer_name, // Name
                        _sub_layer.strips_LV.at(_sub_layer_name), // Logical volume
                        sub_layer_LV, // Mother logival volume
                        kYAxis, // Aare placed along this axis
                        _sub_layer.strip_per_sub_layer, // number of strips
                        _sub_layer_placement[sub_layer_name], // The parametrisation
                        check_overlap // check overlap
                    );
                }
            }

            // Step 3: Update World Size
            _logger->warn("Updating world size");
            _logger->warn("old world size: [{}, {}, {}]", _world_size[0], _world_size[1], _world_size[2]);
            _logger->warn("new tracker region size: [{}, {}, {}]", size[0], size[1], size[2]);

            _world_size = {
                std::max(_world_size[0], size[0] + 2 * std::abs(position[0])) + 1 * cm,
                std::max(_world_size[1], size[1] + 2 * std::abs(position[1])) + 1 * cm,
                std::max(_world_size[2], size[2] + 2 * std::abs(position[2])) + 1 * cm
            };

            _logger->warn("new world size: [{}, {}, {}]", _world_size[0], _world_size[1], _world_size[2]);
        }
    }

    void detector_construction::buildCalorimeter(bool check_overlap) {}
}
