//
// Created by Yulei on 2024/3/8.
//

#include "user-action/run_action.h"
#include "control/database.h"

#include <G4Threading.hh>

namespace SimREX::Simulation {
    run_action::run_action() {
        const auto logger_name =
            G4Threading::IsMasterThread()
                ? "Run Action: Master"
                : std::format("Run Action: {}", G4Threading::G4GetThreadId());

        _logger = GEM::LoggerManager::getInstance()->createLogger(logger_name);

        _logger->info(
            "Creating data manager for thread {}, master? {}",
            G4Threading::G4GetThreadId(), G4Threading::IsMasterThread()
        );
        db::Instance()->registerDataManagers(G4Threading::G4GetThreadId());
    }

    void run_action::BeginOfRunAction(const G4Run*) {
        _logger->info("Begin of run action.");

        // Only the worker thread needs to create a data manager
        if (!G4Threading::IsMasterThread()) {
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
