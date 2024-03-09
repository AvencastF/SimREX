//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_RUN_ACTION_H
#define SIMREX_RUN_ACTION_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include "G4UserRunAction.hh"
#include "G4Threading.hh"

namespace SimREX::Simulation {
    class run_action : public G4UserRunAction {
    public:
        run_action() {
            auto logger_name =
                    G4Threading::IsMasterThread() ?
                    "Run Action: Master" :
                    std::format("Run Action: {}", G4Threading::G4GetThreadId());

            _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger(logger_name);
        };

        ~run_action() override = default;

        void BeginOfRunAction(const G4Run *) override;

        void EndOfRunAction(const G4Run *) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;
    };
}


#endif //SIMREX_RUN_ACTION_H
