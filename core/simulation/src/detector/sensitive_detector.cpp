//
// Created by Yulei on 2024/3/10.
//

#include "detector/sensitive_detector.h"

namespace SimREX::Simulation {
    sensitive_detector::sensitive_detector(const std::string& name, const G4String& output_tree_name, int thread_id)
        : G4VSensitiveDetector(name), _name(name) {

        _logger = GEM::LoggerManager::getInstance()->createLogger(
            fmt::format("Sensitive Detector - {}: ", name, thread_id)
        );
    }

    void sensitive_detector::initializeHits() const {

    }

    G4bool sensitive_detector::ProcessHits(G4Step* aStep, G4TouchableHistory* history) {

        return true;
    }

    void sensitive_detector::EndOfEvent(G4HCofThisEvent* hitCollection) {

    }
}
