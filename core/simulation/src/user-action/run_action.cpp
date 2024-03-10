//
// Created by Yulei on 2024/3/8.
//

#include "user-action/run_action.h"
#include "control/database.h"

#include <G4Threading.hh>

namespace SimREX::Simulation {
    void run_action::BeginOfRunAction(const G4Run*) {
        _logger->info("Begin of run action.");

        // Only the worker thread needs to create a data manager
        if (!G4Threading::IsMasterThread()) {
            std::cout<<"Creating data manager for thread "<<G4Threading::G4GetThreadId()<<std::endl;
            db::Instance()->registerDataManagers(G4Threading::G4GetThreadId());
            db::Instance()->getDataManager(G4Threading::G4GetThreadId())->book();
        }
    }

    void run_action::EndOfRunAction(const G4Run*) {
        if (!G4Threading::IsMasterThread()) {
            db::Instance()->getDataManager(G4Threading::G4GetThreadId())->save();
        }

        _logger->info("End of run action.");
    }
}
