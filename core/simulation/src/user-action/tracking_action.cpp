//
// Created by Yulei on 2024/3/8.
//

#include "user-action/tracking_action.h"

void SimREX::Simulation::tracking_action::PreUserTrackingAction(const G4Track *track) {
    _logger->info("tracking action: pre user tracking action.");

}

void SimREX::Simulation::tracking_action::PostUserTrackingAction(const G4Track *track) {
    _logger->info("tracking action: post user tracking action.");

}
