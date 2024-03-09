//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_WORKER_INITIALIZATION_H
#define SIMREX_WORKER_INITIALIZATION_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include "G4UserWorkerInitialization.hh"
#include "Randomize.hh"
#include "G4Threading.hh"

namespace SimREX::Simulation {
    class worker_initialization : public G4UserWorkerInitialization {
    public:
        explicit worker_initialization(G4long base_seed);

        void WorkerInitialize() const override;


    private:
        std::shared_ptr<spdlog::logger> _logger;

        G4long _base_seed = 0;
    };
}


#endif //SIMREX_WORKER_INITIALIZATION_H
