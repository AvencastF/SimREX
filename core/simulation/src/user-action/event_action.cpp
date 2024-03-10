//
// Created by Yulei on 2024/3/8.
//

#include "user-action/event_action.h"
#include "control/database.h"

// Geant4 dependencies
#include <G4Threading.hh>
#include <G4Event.hh>
#include <G4RunManager.hh>

namespace SimREX::Simulation {
    std::array<int, 4> retrieveNumbers(const std::string& str) {
        std::istringstream iss(str);
        std::string line;
        std::array<int, 4> numbers{};

        // Skip the first two lines
        std::getline(iss, line);
        std::getline(iss, line);

        // Read the next four lines and convert them to integers
        for (int i = 0; i < 4; ++i) {
            std::getline(iss, line);
            numbers.at(i) = std::stoi(line);
        }

        return numbers;
    }

    void event_action::BeginOfEventAction(const G4Event* evt) {
#ifdef DEBUG
        _logger->info("Begin of event {}.", evt->GetEventID());
#else
    _logger->info("Running event {}.",  evt->GetEventID());
#endif

        G4RunManager::GetRunManager()->StoreRandomNumberStatusToG4Event(1);
    }

    void event_action::EndOfEventAction(const G4Event* evt) {

        if (!evt->IsAborted()) {
            // Record random seed numbers
            const G4String& random_numbers_str = G4RunManager::GetRunManager()->GetRandomNumberStatusForThisEvent();
            const auto random_numbers = retrieveNumbers(random_numbers_str);

            const auto data_manager = db::Instance()->getDataManager(G4Threading::G4GetThreadId());
            data_manager->setEventInfo(db::Instance()->get<int>("run_number"), evt->GetEventID(), random_numbers);
            data_manager->fill();

#ifdef DEBUG
            _logger->info("End of event {}.", evt->GetEventID());
#endif
        } else {
            const auto data_manager = db::Instance()->getDataManager(G4Threading::G4GetThreadId());
            data_manager->initialize();

#ifdef DEBUG
            _logger->error("Event {} is aborted.", evt->GetEventID());
#endif
        }
    }
}
