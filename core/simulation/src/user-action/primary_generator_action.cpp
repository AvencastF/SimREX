//
// Created by Yulei on 2024/3/9.
//

#include "user-action/primary_generator_action.h"


#include <G4ParticleTable.hh>
#include <G4SystemofUnits.hh>
#include <G4Threading.hh>

namespace SimREX::Simulation {
    primary_generator_action::primary_generator_action() {
        _logger = GEM::LoggerManager::getInstance()->createLogger(
            std::format("Primary Generator Action: {}", G4Threading::G4GetThreadId()));
#ifdef DEBUG
        _logger->info("constructor called.");
#endif

        _particleSource = new G4GeneralParticleSource();

        // temporary default settings
        //    _particleSource->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("e-"));
        //    _particleSource->SetParticlePosition(G4ThreeVector(0., 0., 0.));
        //    _particleSource->SetCurrentSourceIntensity(1.0);
        //
        //    _particleSource->GetCurrentSource()->SetNumberOfParticles(1);
        //    _particleSource->GetCurrentSource()->SetParticleTime(0.0);
        //    _particleSource->GetCurrentSource()->GetEneDist()->SetMonoEnergy(100.0 * MeV);
    }

    primary_generator_action::~primary_generator_action() {
#ifdef DEBUG
        _logger->info("destructor called.");
#endif

        delete _particleSource;
    }

    void primary_generator_action::GeneratePrimaries(G4Event* anEvent) {
#ifdef DEBUG
        _logger->info("Event {0}: generate primaries.", anEvent->GetEventID());
#endif

        _particleSource->GeneratePrimaryVertex(anEvent);

#ifdef DEBUG
        // Access the list of primary vertices
        G4PrimaryVertex* primaryVertex = anEvent->GetPrimaryVertex();
        // Loop over all primary vertices
        while (primaryVertex) {
            // Get the position of the vertex
            auto position = primaryVertex->GetPosition();
            auto momentum = primaryVertex->GetPrimary()->GetMomentum();
            // Log the vertex position
            _logger->info(
                "Event {0}: Primary vertex:"
                "\n\t - Vertex position: {1}, {2}, {3}"
                "\n\t - Vertex momentum: {4}, {5}, {6}",
                anEvent->GetEventID(),
                position.x(), position.y(), position.z(),
                momentum.x(), momentum.y(), momentum.z()
            );
            // Move to the next vertex
            primaryVertex = primaryVertex->GetNext();
        }
#endif
    }
}
