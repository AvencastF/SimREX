//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_STEPPING_ACTION_H
#define SIMREX_STEPPING_ACTION_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include "G4UserSteppingAction.hh"

namespace SimREX::Simulation {
    class stepping_action : public G4UserSteppingAction {
    public:
        stepping_action() {
            _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("stepping_action");
        }

        ~stepping_action() override = default;

        void UserSteppingAction(const G4Step *step) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;
    };
}


#endif //SIMREX_STEPPING_ACTION_H
