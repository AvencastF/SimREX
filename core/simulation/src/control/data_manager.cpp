//
// Created by Yulei on 2024/3/10.
//

#include "control/data_manager.h"

#include <utility>


namespace SimREX::Simulation {
    data_manager::data_manager(std::string output_file_name, std::string output_tree_name, int thread_id)
        :
        _output_file_name(std::move(output_file_name)),
        _output_tree_name(std::move(output_tree_name)),
        _thread_id(thread_id) {
        _logger = GEM::LoggerManager::getInstance()->createLogger(
            std::format("Data Manager: {}", thread_id)
        );

        _event = new GEM::event();

        _output_file_name = fmt::format("{}.{}", _output_file_name, _thread_id);
    }

    data_manager::~data_manager() {
        _output_file->Close();

        delete _output_file;

        delete _event;
    }

    void data_manager::initialize() {
        _event->initialize();
    }

    void data_manager::book() {
        _logger->info("Booking output file: {} with tree: {}", _output_file_name, _output_tree_name);

        _output_file = new TFile(_output_file_name.c_str(), "RECREATE");
        if (_output_file->IsZombie()) {
            _logger->error("Failed to create output file: {}", _output_file_name);
            exit(EXIT_FAILURE);
        }
        _output_tree = new TTree(_output_tree_name.c_str(), "SimREX Event Tree");

        _output_tree->Branch("run_number", &_run_number, "run_number/I");
        _output_tree->Branch("event_number", &_event_number, "event_number/I");
        _output_tree->Branch("event", "GEM::event", &_event, 320000000);
    }
}
