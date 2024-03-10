//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_TRACKING_ACTION_H
#define SIMREX_TRACKING_ACTION_H

// core dependencies
#include "global-event-model/logger.h"
#include "global-event-model/particle.h"
#include "control/database.h"

// Geant4 dependencies
#include <G4UserTrackingAction.hh>
#include <G4Threading.hh>

namespace SimREX::Simulation {
    class tracking_action final : public G4UserTrackingAction {
    public:
        tracking_action() {
            _logger = GEM::LoggerManager::getInstance()->createLogger(
                    std::format("Tracking Action: {}", G4Threading::G4GetThreadId()));
        }

        ~tracking_action() override = default;

        void PreUserTrackingAction(const G4Track *track) override;

        void PostUserTrackingAction(const G4Track *track) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;

    };
}


#endif //SIMREX_TRACKING_ACTION_H
