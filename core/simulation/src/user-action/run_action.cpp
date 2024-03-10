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
            _data_manager = new data_manager(
                db::Instance()->get<std::string>("data_manager/output_file_name"),
                db::Instance()->get<std::string>("data_manager/output_tree_name"),
                G4Threading::G4GetThreadId()
            );
            _data_manager->initialize();
            _data_manager->book();
        }
    }

    void run_action::EndOfRunAction(const G4Run*) {
        _logger->info("End of run action.");

        if (!G4Threading::IsMasterThread()) {
            delete _data_manager;
        }
    }
}
