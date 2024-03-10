//
// Created by Yulei on 2024/3/8.
//

#include "user-action/tracking_action.h"

#include <G4Track.hh>
#include <G4VProcess.hh>

namespace SimREX::Simulation {
    void addState(GEM::particle* particle, const G4Track* track) {
        auto _state = GEM::particle_state();
        _state.particle = particle;
        _state.position = {
            track->GetPosition().x(), track->GetPosition().y(), track->GetPosition().z(), track->GetGlobalTime()
        };
        _state.momentum = {
            track->GetMomentum().x(), track->GetMomentum().y(), track->GetMomentum().z(), track->GetTotalEnergy()
        };
        _state.dE = 0;

        const auto process_name = "InitialStep";
        _state.process_id = db::Instance()->getProcessMap().at(process_name);

        particle->addState(_state);
    }

    void tracking_action::PreUserTrackingAction(const G4Track* track) {
        auto* dm = db::Instance()->getDataManager(G4Threading::G4GetThreadId());

        auto* _particle = new GEM::particle();
        dm->getParticles()->emplace_back(_particle);

        _particle->setId(track->GetTrackID());
        _particle->setPdg(track->GetDefinition()->GetPDGEncoding());
        _particle->setCharge(track->GetDefinition()->GetPDGCharge());

        // Find parent and link children
        if (track->GetParentID() > 0) {
            if (const auto parent = dm->getEvent()->searchParticle(track->GetParentID()); parent)
                parent->addChild(_particle); // parent will be set in this function at the mean time
        }

        // Add initial state
        addState(_particle, track);
    }

    void tracking_action::PostUserTrackingAction(const G4Track* track) {
        auto* dm = db::Instance()->getDataManager(G4Threading::G4GetThreadId());
        auto* _particle = dm->getEvent()->searchParticle(track->GetTrackID());

        if (!_particle) {
            _logger->error("Particle {0} not found", track->GetTrackID());
            return;
        }

        // Add final state (?, it should be recorded in step action)
        // addState(_particle, track);
    }
}
