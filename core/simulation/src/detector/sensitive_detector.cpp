//
// Created by Yulei on 2024/3/10.
//

#include "detector/sensitive_detector.h"

namespace SimREX::Simulation {
    sensitive_detector::sensitive_detector(const std::string& name, const db::det_type& type, int thread_id)
        : G4VSensitiveDetector(name), _name(name), _type(type) {
        _logger = GEM::LoggerManager::getInstance()->createLogger(
            fmt::format("Sensitive Detector - {}: ", name, thread_id)
        );
    }

    auto sensitive_detector::initializeHits(const G4StepPoint* step, G4TouchableHistory* history) -> GEM::hit* {
        const auto hit = new GEM::hit();

        const G4ThreeVector origin(0., 0., 0.);
        const auto cell_position = history->GetHistory()->GetTopTransform().Inverse().TransformPoint(origin);

        // hit->setCellId();
        // hit->setCellIdX();
        // hit->setCellIdY();
        // hit->setCellIdZ();

        hit->setX(cell_position.x());
        hit->setY(cell_position.y());
        hit->setZ(cell_position.z());


        return hit;
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

        auto hit = initializeHits(_step->GetPreStepPoint(), touchable);


        return true;
    }

    auto sensitive_detector::EndOfEvent(G4HCofThisEvent* hitCollection) -> void {}
}
