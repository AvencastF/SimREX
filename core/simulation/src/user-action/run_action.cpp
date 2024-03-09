//
// Created by Yulei on 2024/3/8.
//

#include "user-action/run_action.h"

#include "G4Threading.hh"

void SimREX::Simulation::run_action::BeginOfRunAction(const G4Run *) {
    _logger->info("Begin of run action.");

}

void SimREX::Simulation::run_action::EndOfRunAction(const G4Run *) {
    _logger->info("End of run action.");
}
