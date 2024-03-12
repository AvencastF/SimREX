//
// Created by Yulei on 2024/3/11.
//

#include "detector/matrix_parameterization.h"

#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <utility>

namespace SimREX::Simulation {
    matrix_parameterization::matrix_parameterization(
        std::string material,
        std::string name,
        G4ThreeVector size,
        G4ThreeVector position,
        tracker_layer rotation,
        double offset,
        int strip_per_layer,
        G4LogicalVolume* mother_LV,
        bool check_overlap
    ) : _type(db::det_type::tracker),
        _tracker({
            std::move(name),
            std::move(material),
            static_cast<double>(offset),
            0,
            strip_per_layer,
            std::move(position),
            std::move(size),
            std::move(rotation)
        }) {
        _logger = GEM::LoggerManager::getInstance()->createLogger(
            std::format("Martrix Parameterization: {}", _tracker.name)
        );

        _tracker.n_sub_layers = _tracker.rotation.size();

        // the strip is placed along y-axis, partitioned along x-axis, the z-axis is the thickness
        _tracker.strip_size = {
            _tracker.size.x(),
            std::floor((_tracker.size.y()) / strip_per_layer / CLHEP::um) * CLHEP::um,
            _tracker.size.z()
        };


        // compute the position of the strip for each sub-layer
        _tracker.strip_position.reserve(strip_per_layer);
        for (int i = 0; i < strip_per_layer; ++i) {
            _tracker.strip_position.emplace_back(
                0,
                _tracker.size.y() / 2. * ((2. * i + 1.) / strip_per_layer - 1.),
                0
            );
        }

        _logger->warn(
            "Tracker: {} with {} strips of {} sub-layers",
            _tracker.name, _tracker.strip_per_sub_layer, _tracker.n_sub_layers
        );
        _logger->warn(
            "Tracker: strip size: {} mm x {} um x {} um",
            _tracker.strip_size.x(), _tracker.strip_size.y() / CLHEP::um, _tracker.strip_size.z() / CLHEP::um
        );

        // Build sub-layers of the tracker layer (not the strip)
        for (int i = 0; i < _tracker.n_sub_layers; ++i) {
            const std::string sub_layer_name = std::format("{}_sub-layer_{}", _tracker.name, i);
            _logger->info("Building sub-layer: {} with name {}", i, sub_layer_name);

            _tracker.sub_layers_LV[sub_layer_name] =
                new G4LogicalVolume(
                    new G4Box(
                        std::format("{}_sub-layer_{}_Box", _tracker.name, i),
                        (_tracker.size.x() + _eps) / 2.,
                        (_tracker.size.y() + _eps) / 2.,
                        (_tracker.size.z() + _eps) / 2.
                    ),
                    G4NistManager::Instance()->FindOrBuildMaterial(_tracker.material),
                    sub_layer_name
                );

            G4ThreeVector layer_position;
            if (i == 0) layer_position = {0, 0, -_tracker.offset};
            if (i == 1) layer_position = {0, 0, _tracker.offset};

            auto* rot = new G4RotationMatrix();
            rot->rotateX(_tracker.rotation[i][0]);
            rot->rotateY(_tracker.rotation[i][1]);
            rot->rotateZ(_tracker.rotation[i][2]);

            new G4PVPlacement(
                rot,
                layer_position,
                _tracker.sub_layers_LV[sub_layer_name],
                std::format("{}_sub-layer", _tracker.name, i),
                mother_LV,
                false,
                i,
                check_overlap
            );

            // build the solid and logic volumes of the strips
            _tracker.strips_LV[sub_layer_name] = new G4LogicalVolume(
                new G4Box(
                    std::format("{}_strip_Box", _tracker.name),
                    (_tracker.strip_size.x()) / 2.,
                    (_tracker.strip_size.y()) / 2.,
                    (_tracker.strip_size.z()) / 2.
                ),
                G4NistManager::Instance()->FindOrBuildMaterial(_tracker.material),
                std::format("{}_strip_LV", _tracker.name)
            );
        }

        // Building strips in sub-layers in the rest class functions
    }

    void matrix_parameterization::ComputeTransformation(G4int copy_num, G4VPhysicalVolume* PV) const {
        const G4ThreeVector origin = _tracker.strip_position.at(copy_num);

        PV->SetTranslation(origin);
    }

    void matrix_parameterization::ComputeDimensions(
        G4Box& unit,
        G4int copy_num,
        const G4VPhysicalVolume* PV
    ) const {}
}
