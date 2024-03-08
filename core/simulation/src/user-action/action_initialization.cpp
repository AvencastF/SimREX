//
// Created by Yulei on 2024/3/8.
//

#include "user-action/action_initialization.h"
#include "user-action/run_action.h"
#include "user-action/event_action.h"
#include "user-action/stepping_action.h"
#include "user-action/tracking_action.h"

void SimREX::Simulation::action_initialization::BuildForMaster() const {
    _logger->info("build for master: register run action.");

    SetUserAction(new run_action());
}

void SimREX::Simulation::action_initialization::Build() const {
    _logger->info("build for each workers.");

    //    SetUserAction(new primary_generator_action());

    SetUserAction(new run_action());
    SetUserAction(new event_action());
    SetUserAction(new tracking_action());
    SetUserAction(new stepping_action());
}

