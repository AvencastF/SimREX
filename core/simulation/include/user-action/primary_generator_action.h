//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_PRIMARY_GENERATOR_ACTION_H
#define SIMREX_PRIMARY_GENERATOR_ACTION_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4GeneralParticleSource.hh>
#include <G4Event.hh>

namespace SimREX::Simulation {
    class primary_generator_action : public G4VUserPrimaryGeneratorAction {
    public:
        primary_generator_action();

        ~primary_generator_action() override;

        void GeneratePrimaries(G4Event* anEvent) override;

    private:
        std::shared_ptr<spdlog::logger> _logger;

        G4GeneralParticleSource* _particleSource;
    };
}


#endif //SIMREX_PRIMARY_GENERATOR_ACTION_H
