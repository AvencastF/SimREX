//
// Created by Yulei on 2024/3/10.
//

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

// core dependencies
#include "global-event-model/logger.h"
#include "control/database.h"
#include "global-event-model/event.h"

// STL dependencies
#include <string>

// ROOT dependencies
#include <TFile.h>
#include <TTree.h>

// Geant4 dependencies
#include <G4Track.hh>
#include <G4Event.hh>

namespace SimREX::Simulation {
    /**
     * Thread-local data manager
     */


    struct event_flags {
        bool abort = false;
    };

    struct track_flags {
        bool record = true;
        bool step = true;
    };

    class data_manager {
    public:
        data_manager() = delete;
        ~data_manager();

        explicit data_manager(std::string output_file_name, std::string output_tree_name, int thread_id);

        //! Initialize the event data in data manager
        void initialize() const;

        //! Set the run number, event number, and the random seed (rndm in GEM::event)
        void setEventInfo(int run_number, int event_number, const std::array<int, 4>& random_number);

        //! Book the output file and tree in ROOT at the beginning of each worker thread (RunAction)
        void book();

        //！For each Sensitive Detector, book the hit collection in GEM::event
        void bookHitCollection(const std::string& col_name) const;

        //! Fill the MC particles in global-data-model event
        void fillParticles();

        //! Fill hits in global-data-model event
        void fillHits(const std::string& col_name, GEM::hit* hit) const;

        //! Fill the entry in the output tree
        void fill();

        //! Save the output file and tree in ROOT at the end of each worker thread (RunAction)
        void save();


        [[nodiscard]] std::vector<GEM::particle*>* getParticles() const {
            return _event->getMCParticles();
        }

        [[nodiscard]] GEM::event* getEvent() const {
            return _event;
        }

        /*
         * For Filters
         */
        [[nodiscard]] event_flags getEventFlags() const {
            return _event_flags;
        }

        [[nodiscard]] track_flags getTrackFlags() const {
            return _track_flags;
        }

        void checkEventFilters(const G4Event* event);

        void checkTrackFilters(const G4Track* track);

        void checkStepFilters(const G4Step* step);

    private:
        std::shared_ptr<spdlog::logger> _logger;
        int _thread_id = -1;

        std::string _output_file_name;
        std::string _output_tree_name;
        TFile* _output_file = nullptr;
        TTree* _output_tree = nullptr;

        GEM::event* _event = nullptr;
        int _event_number = 0;
        int _run_number = 0;

        /*
         * Flags to control the simulation
         */
        event_flags _event_flags;
        track_flags _track_flags;
    };
}


#endif //DATA_MANAGER_H
