//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_DETECTOR_CONSTRUCTION_H
#define SIMREX_DETECTOR_CONSTRUCTION_H

// core dependencies
#include "global-event-model/logger.h"
#include "magnetic-field/magnetic_field.h"

// Geant4 dependencies
#include <G4VUserDetectorConstruction.hh>
#include <G4FieldManager.hh>
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"

namespace SimREX::Simulation {

    class detector_construction : public G4VUserDetectorConstruction {
    public:
        detector_construction();
        ~detector_construction() override;

        G4VPhysicalVolume* Construct() override;
        void ConstructSDandField() override;

    private:
        std::shared_ptr<spdlog::logger> _logger;

        static G4ThreadLocal magnetic_field* _magneticField;
        static G4ThreadLocal G4FieldManager* _fieldMgr;
    };

}


#endif //SIMREX_DETECTOR_CONSTRUCTION_H
