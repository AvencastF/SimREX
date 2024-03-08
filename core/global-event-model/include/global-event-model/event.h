//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_EVENT_H
#define SIMREX_EVENT_H

// STL dependencies
#include <vector>
#include <map>
#include <array>

// ROOT dependencies
#include "TObject.h"
#include "TString.h"

// Global-Event-Model dependencies
#include "global-event-model/hit.h"
#include "global-event-model/particle.h"
#include "global-event-model/logger.h"

using std::vector, std::map, std::array;

namespace SimREX::GEM {

    class event : public TObject {
    public:
        event();

        ~event() override;

        [[nodiscard]] unsigned int getRunNumber() const {
            return _run_number;
        }

        void setRunNumber(unsigned int runNumber) {
            _run_number = runNumber;
        }

        [[nodiscard]] unsigned int getEventNumber() const {
            return _event_number;
        }

        void setEventNumber(unsigned int eventNumber) {
            _event_number = eventNumber;
        }

        [[nodiscard]] const array<double, 4> &getRandomNumber() const {
            return _random_number;
        }

        void setRandomNumber(const array<double, 4> &randomNumber) {
            _random_number = randomNumber;
        }

        [[nodiscard]] double getEventWeight() const {
            return _event_weight;
        }

        void setEventWeight(double eventWeight) {
            _event_weight = eventWeight;
        }

        [[nodiscard]] map <TString, vector<hit *>> *getHitCollections() {
            return &_hit_collections;
        }

        void setHitCollections(const map <TString, vector<hit *>> &hitCollections) {
            _hit_collections = hitCollections;
        }

        [[nodiscard]] vector<particle *> *getMCParticles() {
            return &_mc_particles;
        }

        void setMCParticles(const vector<particle *> &mcParticles) {
            _mc_particles = mcParticles;
        }

        void initialization();

        void registerHitCollection(const TString &col_name);

        void removeHitCollection(const TString &col_name);

        vector<hit *> *getHits(const TString &col_name);

        particle *searchParticle(int id);

    private:


        // ---------------------------
        // General event information
        // ---------------------------

        unsigned _run_number = 0;
        unsigned _event_number = 0;
        // random number seed for this event (used for reproducibility in Geant4)
        array<double, 4> _random_number = {0, 0, 0, 0};

        // Used for event biasing
        double _event_weight = 1.0;


        // ---------------------------
        // Data Collections
        // ---------------------------

        map <TString, vector<hit *>> _hit_collections = {};
        vector<particle *> _mc_particles = {};


        // Logger (not recorded in ROOT file)
        std::shared_ptr<spdlog::logger> _logger = nullptr; //!

    ClassDefOverride(event, 1);
    };

}


#endif //SIMREX_EVENT_H
