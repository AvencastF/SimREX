//
// Created by Yulei on 2024/3/9.
//

#include "detector/detector_construction.h"

#include "G4Material.hh"
#include "G4SystemOfUnits.hh"

namespace SimREX::Simulation {
    G4ThreadLocal magnetic_field *detector_construction::_magneticField = nullptr;
    G4ThreadLocal G4FieldManager *detector_construction::_fieldMgr = nullptr;


    detector_construction::detector_construction() {
        _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("detector_construction");
        _logger->info("constructor called.");
    }

    detector_construction::~detector_construction() {
        _logger->info("destructor called.");
    }

    G4VPhysicalVolume *detector_construction::Construct() {
        _logger->info("construct called.");

        bool checkOverlaps = true;

        // geometries --------------------------------------------------------------
        // experimental hall (world volume)
        auto worldSolid
                = new G4Box("worldBox", 10. * m, 3. * m, 10. * m);
        auto worldLogical
                = new G4LogicalVolume(worldSolid, G4Material::GetMaterial("G4_AIR"), "worldLogical");
        auto worldPhysical = new G4PVPlacement(
                nullptr, G4ThreeVector(), worldLogical, "worldPhysical", nullptr, false, 0, checkOverlaps);

        return worldPhysical;
    }

    void detector_construction::ConstructSDandField() {
        _logger->info("construct SD and field called.");

        // magnetic field ----------------------------------------------------------
        _magneticField = new magnetic_field();
        _fieldMgr = new G4FieldManager();
        _fieldMgr->SetDetectorField(_magneticField);
        _fieldMgr->CreateChordFinder(_magneticField);
        G4bool forceToAllDaughters = true;
//        fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);
    }

}