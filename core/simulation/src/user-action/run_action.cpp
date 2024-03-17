//
// Created by Yulei on 2024/3/8.
//

#include "user-action/run_action.h"
#include "control/database.h"

#include <G4Threading.hh>
#include <G4Run.hh>
#include <G4RunManager.hh>

namespace SimREX::Simulation
{
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

    void run_action::BeginOfRunAction(const G4Run* run) {
        _logger->info("Begin of run action.");

        // Only the worker thread needs to create a data manager
        if (!G4Threading::IsMasterThread()) {
            const auto dm = db::Instance()->getDataManager(G4Threading::G4GetThreadId());
            dm->set_number_of_events(run->GetNumberOfEventToBeProcessed()/G4Threading::GetNumberOfRunningWorkerThreads());
            dm->book();

            // _logger->warn("Number of events to be processed: {}", dm->get_number_of_events());
        }
    }

    void run_action::EndOfRunAction(const G4Run*) {
        if (!G4Threading::IsMasterThread()) {
            db::Instance()->getDataManager(G4Threading::G4GetThreadId())->save();
        }

        _logger->info("End of run action.");
    }
}
