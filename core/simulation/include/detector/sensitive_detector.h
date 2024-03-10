//
// Created by Yulei on 2024/3/10.
//

#ifndef SENSITIVE_DETECTOR_H
#define SENSITIVE_DETECTOR_H

// core dependencies
#include "global-event-model/logger.h"
#include "global-event-model/hit.h"

// Geant4 dependencies
#include <G4VSensitiveDetector.hh>

// STL dependencies
#include <string>
#include <unordered_map>

namespace SimREX::Simulation {
    enum class det_type {
        none,
        tracker,
        calorimeter
    };


    class sensitive_detector final : G4VSensitiveDetector {
    public:
        sensitive_detector() = delete;
        ~sensitive_detector() override = default;

        explicit sensitive_detector(const std::string& name, const G4String& output_tree_name, int thread_id);

        //! Initialize the sensitive detector
        void initializeHits() const;

        //! Process the hits in the sensitive detector
        G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history) override;

        //! End of event action
        void EndOfEvent(G4HCofThisEvent* hitCollection) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;

        det_type _type = det_type::none;
        std::string _name;

        struct ArrayHasher {
            std::size_t operator()(const std::array<int, 3>& arr) const {
                const std::size_t h1 = std::hash<int>{}(arr[0]);
                const std::size_t h2 = std::hash<int>{}(arr[1]);
                const std::size_t h3 = std::hash<int>{}(arr[2]);
                return h1 ^ (h2 << 1) ^ (h3 << 2);
            }
        };
        std::unordered_map<std::array<int, 3>, GEM::hit*, ArrayHasher> _hits;
    };
}


#endif //SENSITIVE_DETECTOR_H
