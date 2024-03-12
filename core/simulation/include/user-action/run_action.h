//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_RUN_ACTION_H
#define SIMREX_RUN_ACTION_H

// core dependencies
#include "global-event-model/logger.h"
#include "control/data_manager.h"

// Geant4 dependencies
#include <G4UserRunAction.hh>
#include <G4Threading.hh>

namespace SimREX::Simulation {
    class run_action : public G4UserRunAction {
    public:
        run_action();

        ~run_action() override = default;

        void BeginOfRunAction(const G4Run*) override;

        void EndOfRunAction(const G4Run*) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;

        data_manager* _data_manager = nullptr;
    };
}


#endif //SIMREX_RUN_ACTION_H
