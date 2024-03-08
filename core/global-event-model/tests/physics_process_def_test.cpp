//
// Created by Yulei on 2024/3/8.
//

#include "global-event-model/physics_process_def.h"
#include <iostream>

void testPhysicsDefInitialization() {
    SimREX::GEM::PhysicsDef physicsDef;

    bool success = true;

    if (physicsDef.dPhyTypeVec.size() != physicsDef.dPhyTypeMap.size()) {
        std::cerr << "Error: Size mismatch between dPhyTypeVec and dPhyTypeMap" << std::endl;
        success = false;
    }

    for (size_t i = 0; i < physicsDef.dPhyTypeVec.size(); ++i) {
        if (physicsDef.dPhyTypeMap.find(physicsDef.dPhyTypeVec[i]) == physicsDef.dPhyTypeMap.end()) {
            std::cerr << "Error: dPhyTypeVec element not found in dPhyTypeMap" << std::endl;
            success = false;
            break;
        }

        if (physicsDef.dPhyTypeMap[physicsDef.dPhyTypeVec[i]] != static_cast<int>(i)) {
            std::cerr << "Error: Incorrect mapping for " << physicsDef.dPhyTypeVec[i] << std::endl;
            success = false;
            break;
        }
    }

    if (success) {
        std::cout << "PhysicsDef initialization test passed." << std::endl;
    } else {
        std::cerr << "PhysicsDef initialization test failed." << std::endl;
    }
}

int main() {
    testPhysicsDefInitialization();
    return 0;
}
