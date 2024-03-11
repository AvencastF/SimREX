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
    public:
        explicit matrix_parameterization(db::det_type type);
        ~matrix_parameterization() override = default;

        //! Compute the transformation of the physical volume for each copy number
        void ComputeTransformation(G4int copy_num, G4VPhysicalVolume*) const override;

        //! Compute the dimensions of the physical volume for each copy number
        void ComputeDimensions(G4Box& unit, G4int copy_num, const G4VPhysicalVolume* PV) const override;

    private:
        std::shared_ptr<spdlog::logger> _logger;

        db::det_type _type = db::det_type::none;


        //! Dummy declarations to get rid of warnings ... (no usage of these functions)
        void ComputeDimensions (G4Trd&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Trap&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Cons&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Sphere&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Orb&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Ellipsoid&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Torus&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Para&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Hype&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Polycone&,const G4int,
                                const G4VPhysicalVolume*) const override {}
        void ComputeDimensions (G4Polyhedra&,const G4int,
                                const G4VPhysicalVolume*) const override {}
    };
}

#endif //MATRIX_PARAMETERIZATION_H
