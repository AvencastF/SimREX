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

namespace SimREX::Simulation {
    class data_manager {
    public:
        data_manager() = delete;
        ~data_manager();

        explicit data_manager(std::string output_file_name, std::string output_tree_name, int thread_id);

        //! Initialize the event data in data manager
        void initialize();

        //! Book the output file and tree in ROOT at the beginning of each worker thread (RunAction)
        void book();

        //!

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
    };
}


#endif //DATA_MANAGER_H
