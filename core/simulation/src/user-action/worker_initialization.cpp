//
// Created by Yulei on 2024/3/9.
//

#include "user-action/worker_initialization.h"


namespace SimREX::Simulation
{
    void worker_initialization::WorkerInitialize() const {
        const G4int threadID = G4Threading::G4GetThreadId();
        const G4long unique_seed = _base_seed + threadID;

        G4Random::setTheEngine(new CLHEP::RanecuEngine);
        G4Random::setTheSeed(unique_seed);

#ifdef DEBUG
        _logger->info("Worker initialized with seed {0} (base seed: {1}).", unique_seed, _base_seed);
#endif
    }

    void worker_initialization::WorkerStop() const {
        GEM::LoggerManager::getInstance()->flush();
    }

    worker_initialization::worker_initialization(G4long base_seed) : _base_seed(base_seed) {
        _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("Worker Initialization");
    }
}
