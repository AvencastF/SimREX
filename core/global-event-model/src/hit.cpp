//
// Created by Yulei on 2024/3/7.
//

#include "global-event-model/hit.h"

SimREX::GEM::hit::~hit() {
    auto clear = [](auto &v) {
        v.clear();
        v.shrink_to_fit();
    };

    clear(_e);
    clear(_t);
    clear(_particle_contributions);
}

void SimREX::GEM::hit::addParticleContribution(SimREX::GEM::particle_state *state) {
    _particle_contributions.push_back(state);

    if (state->hit != this) {
        state->hit = this;
    }
}

