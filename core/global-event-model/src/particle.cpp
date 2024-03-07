//
// Created by Yulei on 2024/3/8.
//

#include "global-event-model/particle.h"


void SimREX::GEM::particle::addChild(SimREX::GEM::particle *child) {
    _children.push_back(child);

    if (child->getParent() != this) {
        child->setParent(this);
    }
}

void SimREX::GEM::particle::removeChild(SimREX::GEM::particle *child) {
    auto it = std::find(_children.begin(), _children.end(), child);

    if (it != _children.end()) {
        _children.erase(it);

        if (child->getParent() == this) {
            child->setParent(nullptr);
        }
    }
}

SimREX::GEM::particle::~particle() {
    auto clear = [](auto &v) {
        v.clear();
        v.shrink_to_fit();
    };

    clear(_children);
    clear(_states);
}


