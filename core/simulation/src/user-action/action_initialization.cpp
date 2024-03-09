//
// Created by Yulei on 2024/3/8.
//

#include <G4RunManager.hh>
#include <G4EventManager.hh>
#include <G4TrackingManager.hh>
#include <G4SteppingManager.hh>
#include <G4Threading.hh>

#include "control/control.h"

#include "user-action/action_initialization.h"
#include "user-action/run_action.h"
#include "user-action/event_action.h"
#include "user-action/stepping_action.h"
#include "user-action/tracking_action.h"
#include "user-action/primary_generator_action.h"


namespace SimREX::Simulation {
    void action_initialization::BuildForMaster() const {
#ifdef DEBUG
        _logger->info("build for master: register run action.");
#endif

        SetUserAction(new run_action());

        // Set Verbosity
        G4RunManager *runManager = G4RunManager::GetRunManager();
        runManager->SetVerboseLevel(control::Instance()->getValue<int>("verbosity/run"));
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

        G4EventManager *eventManager = G4EventManager::GetEventManager();
        G4TrackingManager *trackingManager = eventManager->GetTrackingManager();
        G4SteppingManager *steppingManager = trackingManager->GetSteppingManager();
        eventManager->SetVerboseLevel(control::Instance()->getValue<int>("verbosity/event"));
        trackingManager->SetVerboseLevel(control::Instance()->getValue<int>("verbosity/tracking"));
        steppingManager->SetVerboseLevel(control::Instance()->getValue<int>("verbosity/stepping"));
    }

}


