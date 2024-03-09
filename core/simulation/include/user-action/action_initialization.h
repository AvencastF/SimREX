//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_ACTION_INITIALIZATION_H
#define SIMREX_ACTION_INITIALIZATION_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include "G4VUserActionInitialization.hh"

namespace SimREX::Simulation {
    class action_initialization : public G4VUserActionInitialization {
    public:
        action_initialization() {
            _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("Action Initialization");
        }

        ~action_initialization() override = default;

        void BuildForMaster() const override;

        void Build() const override;

    private:
        std::shared_ptr<spdlog::logger> _logger;
    };
}


#endif //SIMREX_ACTION_INITIALIZATION_H
