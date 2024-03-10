//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_PHYSICS_PROCESS_DEF_H
#define SIMREX_PHYSICS_PROCESS_DEF_H

// STL dependencies
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

// Global-Event-Model dependencies
#include "global-event-model/logger.h"

using std::cout, std::endl, std::cerr;
using std::unordered_map, std::vector, std::string;


namespace SimREX::GEM {
    struct PhysicsDef {
        /** the vector of process types */
        vector<string> dPhyTypeVec = {
            "Unknown", // 0
            "Transportation", // 1
            "msc", // 2
            "electronNuclear", // 3
            "Decay", // 4
            "conv", // 5
            "pi+Inelastic", // 6
            "eBrem", // 7
            "neutronInelastic", // 8
            "lambdaInelastic", // 9
            "dInelastic", // 10
            "protonInelastic", // 11
            "hBertiniCaptureAtRest", // 12
            "photonNuclear", // 13
            "tInelastic", // 14
            "kaon-Inelastic", // 15
            "pi-Inelastic", // 16
            "He3Inelastic", // 17
            "compt", // 18
            "kaon0LInelastic", // 19
            "kaon0SInelastic", // 20
            "eIoni", // 21
            "kaon+Inelastic", // 22
            "muMinusCaptureAtRest", // 23
            "sigma-Inelastic", // 24
            "alphaInelastic", // 25
            "sigma+Inelastic", // 26
            "annihil", // 27
            "ionInelastic", // 28
            "hadElastic", // 29
            "phot", // 30
            "xi0Inelastic", // 31
            "nCapture", // 32
            "hIoni", // 33
            "ionIoni", // 34
            "CoulombScat", // 35
            "DMProcessDMBrem", // 36
            "Rayl", // 37
            "muIoni", // 38
            "muPairProd", // 39
            "muBrems", // 40
            "GammaToMuPair", // 41
            "muonNuclear", // 42
            "hPairProd", // 43
            "nKiller", // 44
            "InitialStep", // 45
            "OutOfWorld", // 46
        };
        /** the map of process types */
        unordered_map<string, int> dPhyTypeMap;

        /**Default constructor*/
        PhysicsDef() {
            auto* logger_manager = LoggerManager::getInstance();
            auto logger = logger_manager->createLogger("Physics Process Def");

            for (unsigned int i = 0; i < dPhyTypeVec.size(); ++i) {
                dPhyTypeMap.insert({dPhyTypeVec.at(i), i});

                logger->info("Physics Process Type: {0} ==> {1}", i, dPhyTypeVec.at(i));
            }

            logger->info("Construction completed with {0} processes.", dPhyTypeVec.size());
        }
    };
}

#endif //SIMREX_PHYSICS_PROCESS_DEF_H
