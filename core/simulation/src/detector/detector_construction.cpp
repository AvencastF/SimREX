//
// Created by Yulei on 2024/3/9.
//

#include "detector/detector_construction.h"

#include <ranges>

#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4VisAttributes.hh>

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
    }

    G4VPhysicalVolume* detector_construction::Construct() {
        _logger->info("construct called.");

        bool checkOverlaps = true;

        // geometries
        buildWorldVolume();

        buildTarget();

        buildTracker();

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
        // magnetic field ----------------------------------------------------------
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

        // Build Tracker Region Mannually
        for (
            auto tracker_regions = db::Instance()->get<std::vector<tracker_region>>("tracker-like");
            auto& [name, material, position, daughters, size] : tracker_regions
        ) {
            if (std::ranges::find(build_list, name) == build_list.end()) {
                _logger->warn("tracker region [{}] not in the building list, IGNORE", name);
                continue;
            }

            _logger->info("building tracker region [{}]", name);

            auto pre_computing_size = [name, daughters, this]() {
                std::map<std::string, std::vector<double>> size_map = {
                    {name, {0, 0, 0}}
                };

                // For each daughter, compute the max size
                for (const auto& trk : daughters) {
                    // Step 1: Find the index of the max of abs of the position (mother need to be square)
                    auto find_max_index = [trk, this](int axis) {
                        assert(trk.position.size() == trk.size.size());

                        const bool use_rotation = !trk.rotation.empty();

                        const auto max_position_size = std::ranges::max(
                            trk.position | std::views::transform(
                                [&](const auto& pos) {
                                    const double x = trk.size[&pos - &trk.position[0]][0] / 2.0;
                                    const double y = trk.size[&pos - &trk.position[0]][1] / 2.0;
                                    const double z = trk.size[&pos - &trk.position[0]][2] / 2.0;

                                    if (use_rotation) {
                                        // default geometry is box, calculate the 8 vertices
                                        std::vector<XYZVector> vertices = {
                                            {x, y, z},
                                            {x, -y, z},
                                            {x, y, -z},
                                            {x, -y, -z},
                                            {-x, y, z},
                                            {-x, -y, z},
                                            {-x, y, -z},
                                            {-x, -y, -z}
                                        };

                                        // define rotation
                                        const auto rot_x = RotationX(trk.rotation[&pos - &trk.position[0]][0]);
                                        const auto rot_y = RotationY(trk.rotation[&pos - &trk.position[0]][1]);
                                        const auto rot_z = RotationZ(trk.rotation[&pos - &trk.position[0]][2]);

                                        double max_len = 0;
                                        for (const auto& vertex : vertices) {
                                            XYZVector new_vertex = rot_z(rot_y(rot_x(vertex)));
                                            if (axis == 0) max_len = std::max(max_len, std::abs(new_vertex.x()));
                                            if (axis == 1) max_len = std::max(max_len, std::abs(new_vertex.y()));
                                            if (axis == 2) max_len = std::max(max_len, std::abs(new_vertex.z()));
                                        }
                                        return std::abs(pos[axis]) + max_len;
                                    }
                                    return std::abs(pos[axis]) + trk.size[&pos - &trk.position[0]][axis] / 2.0;
                                }
                            )
                        );
                        // return the maximum position+size (convert half length to size) for each daughter
                        return max_position_size * 2 + _eps;
                    };

                    size_map[trk.name] = {
                        find_max_index(0),
                        find_max_index(1),
                        find_max_index(2)
                    };

                    size_map[name] = {
                        std::max(size_map[name][0], find_max_index(0)),
                        std::max(size_map[name][1], find_max_index(1)),
                        std::max(size_map[name][2], find_max_index(2))
                    };
                }

                return size_map;
            };

            auto size_map = pre_computing_size();
            size = size_map[name];
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
            for (const auto& trk : daughters) {
                auto daughter_size = size_map[trk.name];

                const auto trk_solid = new G4Box(
                    std::format("{}_box", trk.name),
                    daughter_size[0] / 2., daughter_size[1] / 2., daughter_size[2] / 2.
                );
                const auto trk_mat = G4NistManager::Instance()->FindOrBuildMaterial(trk.material);
                const auto trk_LV = new G4LogicalVolume(
                    trk_solid, trk_mat, std::format("{}_LV", trk.name));


                new G4PVPlacement(
                    nullptr, G4ThreeVector(0, 0, 0),
                    trk_LV, trk.name, tracker_region_LV, false, 0, check_overlap
                );
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
