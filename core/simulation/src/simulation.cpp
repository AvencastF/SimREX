//
// Created by Yulei on 2024/3/8.
//

// core dependencies
#include "simulation.h"
#include "control/database.h"
#include "global-event-model/logger.h"
#include "user-action/action_initialization.h"
#include "detector/detector_construction.h"
#include "user-action/worker_initialization.h"

// ROOT dependencies
#include <TROOT.h>

// Geant4 dependencies
#include <G4RunManagerFactory.hh>
#include <FTFP_BERT.hh>
#include <Randomize.hh>

namespace SimREX::Simulation {
    int run_simulation(const std::string& _config_path, int _beam_on, int _random_seed, int _threads) {
        auto _logger = GEM::LoggerManager::getInstance()->createLogger("simulation");

        // Initialize ROOT environment
        gROOT->Reset();
        ROOT::EnableThreadSafety();

        // Initialize the control
        db::Instance()->readYAML(_config_path);

        // Overwrite config with command line arguments
        if (_beam_on >= 0) {
            db::Instance()->set("beam_on", _beam_on);
        }
        if (_random_seed >= 0) {
            db::Instance()->set("random_seed", _random_seed);
        }
        if (_threads >= 0) {
            db::Instance()->set("threads", _threads);
        }
        db::Instance()->printData();

        auto random_seed = db::Instance()->get<int>("random_seed");
        auto beam_on = db::Instance()->get<int>("beam_on");

        // Set random seed for the main program (MT - not for workers)
        G4Random::setTheEngine(new CLHEP::RanecuEngine);
        G4Random::setTheSeed(random_seed);

        // Construct the default run manager
        const auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

#ifdef G4MULTITHREADED
        // Random seed is set for each worker individually ( random_seed + threadID )
        runManager->SetUserInitialization(new worker_initialization(random_seed));
        auto totalThreads = G4Threading::G4GetNumberOfCores();
        const auto threads = db::Instance()->get<int>("threads");
        auto nThreads = threads == 0 ? totalThreads : threads;
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

        // Read and set GPS (set after primary generator action is initialized)
        db::Instance()->readAndSetGPS();

        runManager->BeamOn(beam_on);

        /* Job termination
        * Free the store: user actions, physics_list and detector_description are
        * owned and deleted by the run manager, so they should not be deleted
        * in the main() program !
        */
        delete runManager;

        return 1;
    }
}
