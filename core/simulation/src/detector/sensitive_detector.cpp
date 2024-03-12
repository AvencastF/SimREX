//
// Created by Yulei on 2024/3/10.
//

#include "detector/sensitive_detector.h"

#include <charconv>

namespace SimREX::Simulation {
    sensitive_detector::sensitive_detector(const std::string& name, const db::det_type& type, int thread_id)
        : G4VSensitiveDetector(name), _name(name), _type(type) {
        _logger = GEM::LoggerManager::getInstance()->createLogger(
            fmt::format("Sensitive Detector - {}: {}", name, thread_id)
        );

        _logger->info("{} is created", name);

        if (!G4Threading::IsMasterThread()) {
            _data_manager = db::Instance()->getDataManager(thread_id);
            _data_manager->bookHitCollection(_name);

            _logger->info("{} is booked.", name);
        }
        else {
            _logger->info("Master thread does not need to book hit collection [no real data processed].");
        }

        auto findNumberAfterString = [](const std::string& str, const std::string& target, const char delimiter) {
            if (std::size_t pos = str.find(target); pos != std::string::npos) {
                pos += target.size(); // Move past the target string
                pos = str.find(delimiter, pos) + 1; // Find the delimiter and move to the position after it
                if (pos != std::string::npos && pos < str.size()) {
                    int number;
                    if (auto [ptr, ec]{std::from_chars(&str[pos], &str[str.size()], number)}; ec == std::errc()) {
                        // Check if the conversion succeeded
                        return number;
                    }
                }
            }
            return -1; // Return -1 if no number is found
        };

        _layer = findNumberAfterString(name, "layer", '_');
        _sub_layer = findNumberAfterString(name, "sub-layer", '_');
        _logger->info("Layer: {}, Sub-layer: {}", _layer, _sub_layer);
    }

    void sensitive_detector::initializeHits(GEM::hit* hit, const G4StepPoint* step, G4TouchableHistory* history) {
        const G4ThreeVector origin(0., 0., 0.);
        const auto cell_position = history->GetHistory()->GetTopTransform().Inverse().TransformPoint(origin);

        // hit->setCellId();
        // hit->setCellIdX();
        // hit->setCellIdY();
        // hit->setCellIdZ();

        hit->setX(cell_position.x());
        hit->setY(cell_position.y());
        hit->setZ(cell_position.z());
    }

    auto sensitive_detector::ProcessHits(G4Step* _step, G4TouchableHistory* history) -> G4bool {
        // energy deposit
        G4double e_dep = _step->GetTotalEnergyDeposit();

        // step length
        G4double step_length = 0.;
        if (_step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) {
            step_length = _step->GetStepLength();
        }

        if (e_dep == 0. && step_length == 0.) return false;

        auto* touchable = const_cast<G4TouchableHistory*>(_step->GetPreStepPoint()->GetTouchable());

        const auto hit = new GEM::hit();
        hit->setStage(GEM::stage::simulated);

        _id += 1;
        hit->setId(_id);

        if (_type == db::det_type::tracker) {
            hit->setDetector(GEM::detector::tracker);
            hit->setCellIdX(0);
            hit->setCellIdY(touchable->GetReplicaNumber(0));
            hit->setCellIdZ(_sub_layer);
            hit->setCellId(_layer);

            hit->getE().push_back(e_dep);
            hit->getT().push_back(_step->GetTrack()->GetGlobalTime());

            initializeHits(hit, _step->GetPreStepPoint(), touchable);
        }

        // Link hit and the MC particle states
        const auto particle_id = _step->GetTrack()->GetTrackID();
        if (const auto particle = _data_manager->getEvent()->searchParticle(particle_id)) {
            particle->addHitCache(hit);
        }

        // Fill to Data Manager
        if (_type == db::det_type::tracker) {
            _data_manager->fillHits(_name, hit);
        }

        return true;
    }

    auto sensitive_detector::EndOfEvent(G4HCofThisEvent* hitCollection) -> void {}
}
