//
// Created by Yulei on 2024/3/11.
//

#ifndef MATRIX_PARAMETERIZATION_H
#define MATRIX_PARAMETERIZATION_H

// core dependencies
#include "global-event-model/logger.h"
#include "control/database.h"

// Geant4 dependencies
#include <G4VPVParameterisation.hh>

namespace SimREX::Simulation {
    class matrix_parameterization final : public G4VPVParameterisation {
        struct tracker;

    public:
        matrix_parameterization() = default;
        ~matrix_parameterization() override = default;

        explicit matrix_parameterization(
            std::string material, std::string name,
            G4ThreeVector size, G4ThreeVector position,
            tracker_layer rotation,
            double offset, int strip_per_layer,
            G4LogicalVolume* mother_LV,
            bool check_overlap = false
        );

        //! Compute the transformation of the physical volume for each copy number
        void ComputeTransformation(G4int copy_num, G4VPhysicalVolume*) const override;

        //! Compute the dimensions of the physical volume for each copy number
        void ComputeDimensions(G4Box& unit, G4int copy_num, const G4VPhysicalVolume* PV) const override;

        [[nodiscard]] const tracker& get_tracker() const {
            return _tracker;
        }

    private:
        std::shared_ptr<spdlog::logger> _logger;

        double _eps = 1e-3 * CLHEP::mm;

        db::det_type _type = db::det_type::none;

        struct tracker {
            std::string name;
            std::string material;
            double offset;
            unsigned n_sub_layers;
            int strip_per_sub_layer;
            G4ThreeVector position;
            G4ThreeVector size;

            // determine the size of sub-layer
            tracker_layer rotation;
            // box size of the strip
            G4ThreeVector strip_size;
            // position of the strip
            std::vector<G4ThreeVector> strip_position;

            // the logical volume of the tracker
            std::map<std::string, G4LogicalVolume*> sub_layers_LV;
            std::map<std::string, G4LogicalVolume*> strips_LV;
        } _tracker;


        //! Dummy declarations to get rid of warnings ... (no usage of these functions)
        void ComputeDimensions(G4Trd&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Trap&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Cons&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Sphere&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Orb&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Ellipsoid&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Torus&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Para&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Hype&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Polycone&, const G4int, const G4VPhysicalVolume*) const override {}

        void ComputeDimensions(G4Polyhedra&, const G4int, const G4VPhysicalVolume*) const override {}
    };
}

#endif //MATRIX_PARAMETERIZATION_H
