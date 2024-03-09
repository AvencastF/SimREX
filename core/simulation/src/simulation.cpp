//
// Created by Yulei on 2024/3/8.
//

// core dependencies
#include "simulation.h"
#include "global-event-model/logger.h"
#include "user-action/action_initialization.h"
#include "detector/detector_construction.h"

// Geant4 dependencies
#include <G4RunManagerFactory.hh>
#include <FTFP_BERT.hh>

namespace SimREX::Simulation {

    int run_simulation() {
        auto _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("simulation");

        // Construct the default run manager
        //
        auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

#ifdef G4MULTITHREADED
        // Get number of threads in server
        auto totalThreads = G4Threading::G4GetNumberOfCores();
        auto nThreads = static_cast<int>(totalThreads / 2.);
        runManager->SetNumberOfThreads(nThreads);
        _logger->warn("Using {0}/{1} threads.", nThreads, totalThreads);
#endif

        runManager->SetUserInitialization(new detector_construction());

        // Physics list
        auto physicsList = new FTFP_BERT;
        physicsList->SetVerboseLevel(0);
        runManager->SetUserInitialization(physicsList);

        // User action initialization
        runManager->SetUserInitialization(new action_initialization());

        // Initialize G4 kernel
        runManager->Initialize();

        runManager->BeamOn(48);

        /* Job termination
        * Free the store: user actions, physics_list and detector_description are
        * owned and deleted by the run manager, so they should not be deleted
        * in the main() program !
        */
        delete runManager;

        return 1;
    }
}