//
// Created by Yulei on 2024/3/8.
//

#include "global-event-model/particle.h"


namespace SimREX::GEM {
    void particle::addChild(particle* child) {
        _children.emplace_back(child);

        if (child->getParent() != this) {
            child->setParent(this);
        }
    }

    void particle::removeChild(particle* child) {
        if (const auto it = std::ranges::find(_children, child); it != _children.end()) {
            _children.erase(it);

            if (child->getParent() == this) {
                child->setParent(nullptr);
            }
        }
    }

    particle::~particle() {
        auto clear = [](auto& v) {
            v.clear();
            v.shrink_to_fit();
        };

        clear(_children);
        clear(_states);
    }

    void particle::addState(particle_state state) {
        _states.emplace_back(state);

        if (state.particle != this) {
            state.particle = this;
        }
    }
}
