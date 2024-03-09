//
// Created by Yulei on 2024/3/8.
//

#include "control/control.h"

#include "user-action/action_initialization.h"
#include "user-action/run_action.h"
#include "user-action/event_action.h"
#include "user-action/stepping_action.h"
#include "user-action/tracking_action.h"
#include "user-action/primary_generator_action.h"

#include "G4Threading.hh"

namespace SimREX::Simulation {
    void action_initialization::BuildForMaster() const {
#ifdef DEBUG
        _logger->info("build for master: register run action.");
#endif

        SetUserAction(new run_action());
    }

    void action_initialization::Build() const {
#ifdef DEBUG
        _logger->info("build for thread - {0}", G4Threading::G4GetThreadId());
#endif

        SetUserAction(new primary_generator_action());

        SetUserAction(new run_action());
        SetUserAction(new event_action());
        SetUserAction(new tracking_action());
        SetUserAction(new stepping_action());

    }

}


