//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_EVENT_ACTION_H
#define SIMREX_EVENT_ACTION_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include "G4UserEventAction.hh"


namespace SimREX::Simulation {
    class event_action : public G4UserEventAction {
    public:
        event_action() {
            _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("event_action");
        }

        ~event_action() override = default;

        void BeginOfEventAction(const G4Event *) override;

        void EndOfEventAction(const G4Event *) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;
    };
}


#endif //SIMREX_EVENT_ACTION_H
