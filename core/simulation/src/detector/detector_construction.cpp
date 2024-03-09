//
// Created by Yulei on 2024/3/9.
//

#include "detector/detector_construction.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

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
        G4Material *world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

        auto worldSolid
                = new G4Box("worldBox", 10. * m, 3. * m, 10. * m);
        auto worldLogical
                = new G4LogicalVolume(worldSolid, world_mat, "worldLogical");
        auto worldPhysical = new G4PVPlacement(
                nullptr, G4ThreeVector(), worldLogical, "worldPhysical", nullptr, false, 0, checkOverlaps);

        return worldPhysical;
    }

    void detector_construction::ConstructSDandField() {
#ifdef DEBUG
        _logger->info("Thread - {}: construct SD and field called.", G4Threading::G4GetThreadId());
#endif
        // magnetic field ----------------------------------------------------------
        _magneticField = new magnetic_field();
        _fieldMgr = new G4FieldManager();
        _fieldMgr->SetDetectorField(_magneticField);
        _fieldMgr->CreateChordFinder(_magneticField);
        G4bool forceToAllDaughters = true;
//        fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);
    }

}