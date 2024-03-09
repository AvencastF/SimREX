//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_EVENT_ACTION_H
#define SIMREX_EVENT_ACTION_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include "G4UserEventAction.hh"
#include "G4Threading.hh"


namespace SimREX::Simulation {
    class event_action : public G4UserEventAction {
    public:
        event_action() {
            _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger(std::format("Event Action: {}", G4Threading::G4GetThreadId()));
        }

        ~event_action() override = default;

        void BeginOfEventAction(const G4Event *evt) override;

        void EndOfEventAction(const G4Event *evt) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;
    };
}


#endif //SIMREX_EVENT_ACTION_H
