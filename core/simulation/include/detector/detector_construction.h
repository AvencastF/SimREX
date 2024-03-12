//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_DETECTOR_CONSTRUCTION_H
#define SIMREX_DETECTOR_CONSTRUCTION_H

// core dependencies
#include "global-event-model/logger.h"
#include "magnetic-field/magnetic_field.h"
#include "control/database.h"
#include "detector/matrix_parameterization.h"

// Geant4 dependencies
#include <G4VUserDetectorConstruction.hh>
#include <G4FieldManager.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4UserLimits.hh>

namespace SimREX::Simulation {
    class detector_construction final : public G4VUserDetectorConstruction {
    public:
        detector_construction();
        ~detector_construction() override;

        G4VPhysicalVolume* Construct() override;
        void ConstructSDandField() override;

        //! Build World Volume
        void buildWorldVolume(bool check_overlap = false);

        //! Build Target
        void buildTarget(bool check_overlap = false);

        //! Build Tracker
        void buildTracker(bool check_overlap = false);

        //! Build Calorimeter
        void buildCalorimeter(bool check_overlap = false);

    private:
        double _eps = 1e-3 * mm;

        std::shared_ptr<spdlog::logger> _logger;

        G4LogicalVolume* _world_LV = nullptr;
        G4PVPlacement* _world_PV = nullptr;

        std::array<double, 3> _world_size = {0, 0, 0};

        std::map<std::string, matrix_parameterization*> _sub_layer_placement;

        static G4ThreadLocal magnetic_field* _magneticField;
        static G4ThreadLocal G4FieldManager* _fieldMgr;
    };
}


#endif //SIMREX_DETECTOR_CONSTRUCTION_H
