//
// Created by Yulei on 2024/3/8.
//

#include "user-action/event_action.h"

void SimREX::Simulation::event_action::BeginOfEventAction(const G4Event *) {
    _logger->info("Begin of event action.");
}

void SimREX::Simulation::event_action::EndOfEventAction(const G4Event *) {
    _logger->info("End of event action.");
}
