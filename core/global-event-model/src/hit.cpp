//
// Created by Yulei on 2024/3/7.
//

#include "global-event-model/hit.h"

SimREX::GEM::hit::~hit() {
    auto clear = [](auto& v) {
        v.clear();
        v.shrink_to_fit();
    };

    clear(_e);
    clear(_t);

    _particle_contributions.clear();
    _particle_contributions.shrink_to_fit();
}

void SimREX::GEM::hit::addParticleContribution(SimREX::GEM::particle_state* state) {
    _particle_contributions.emplace_back(state);

    if (state->hit != this) {
        state->hit = this;
    }
}
