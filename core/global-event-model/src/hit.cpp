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
}
