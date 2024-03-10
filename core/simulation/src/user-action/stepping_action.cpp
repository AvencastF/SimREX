//
// Created by Yulei on 2024/3/8.
//

#include "user-action/stepping_action.h"

#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4VProcess.hh>

#include "control/database.h"

namespace SimREX::Simulation {
    void addState(GEM::particle* particle, const G4Step* step) {
        const auto pre_step = step->GetPreStepPoint();
        const auto post_step = step->GetPostStepPoint();

        auto _state = GEM::particle_state();
        _state.particle = particle;
        _state.position = {
            post_step->GetPosition().x(), post_step->GetPosition().y(), post_step->GetPosition().z(),
            post_step->GetGlobalTime()
        };
        _state.momentum = {
            post_step->GetMomentum().x(), post_step->GetMomentum().y(), post_step->GetMomentum().z(),
            post_step->GetTotalEnergy()
        };
        _state.dE = pre_step->GetTotalEnergy() - post_step->GetTotalEnergy();

        const auto process_name = post_step->GetProcessDefinedStep()->GetProcessName();
        _state.process_id = db::Instance()->getProcessMap().at(process_name);

        particle->addState(_state);
    }

    void stepping_action::UserSteppingAction(const G4Step* step) {
        auto* dm = db::Instance()->getDataManager(G4Threading::G4GetThreadId());
        const auto track_id = step->GetTrack()->GetTrackID();

        auto* particle = dm->getEvent()->searchParticle(track_id);
        if (!particle) {
            _logger->error("Particle {0} not found", track_id);
            return;
        }

        // addState(particle, step);
    }
}
