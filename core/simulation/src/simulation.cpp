//
// Created by Yulei on 2024/3/8.
//

// core dependencies
#include "simulation.h"
#include "global-event-model/logger.h"
#include "user-action/action_initialization.h"

// Geant4 dependencies
#include <G4RunManagerFactory.hh>
#include <QBBC.hh>

namespace SimREX::Simulation {

    int run_simulation() {

        // Construct the default run manager
        //
        auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

        // Set mandatory initialization classes
        //
        // Detector construction
//        runManager->SetUserInitialization(new DetectorConstruction());

        // Physics list
        auto physicsList = new QBBC;
        physicsList->SetVerboseLevel(1);
        runManager->SetUserInitialization(physicsList);

        // User action initialization
        runManager->SetUserInitialization(new action_initialization());


        /* Job termination
        * Free the store: user actions, physics_list and detector_description are
        * owned and deleted by the run manager, so they should not be deleted
        * in the main() program !
        */
        delete runManager;

        return 1;
    }
}