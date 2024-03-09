//
// Created by Yulei on 2024/3/8.
//

#include "user-action/event_action.h"

#include "G4Threading.hh"
#include "G4Event.hh"

void SimREX::Simulation::event_action::BeginOfEventAction(const G4Event *evt) {
#ifdef DEBUG
    _logger->info("Begin of event {}.",  evt->GetEventID());
#else
    _logger->info("Running event {}.",  evt->GetEventID());
#endif
}

void SimREX::Simulation::event_action::EndOfEventAction(const G4Event *evt) {
#ifdef DEBUG
    _logger->info("End of event {}.",  evt->GetEventID());
#endif
}
