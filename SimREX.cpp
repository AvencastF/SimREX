//
// Created by Yulei on 2024/3/8.
//

// STL dependencies
#include <iostream>
#include <format>

// argparse dependencies
#include "argparse/argparse.hpp"

// core dependencies
#include "simulation.h"
#include "global-event-model/logger.h"


auto LoggerManager = SimREX::GEM::LoggerManager::getInstance();
auto logger = LoggerManager->createLogger("SimREX");

struct SimArgs final : argparse::Args {
    std::string& config_path = arg("config_path", "Path to the configuration file. [yaml file]");
    int& beam_on = kwarg(
        "b,beam-on", "Number of beam-on events. [override configs]",/*implicit*/"100"
    ).set_default(-1);
    int& random_seed = kwarg(
        "r,random-seed", "Random seed for whole simulation. [override configs]"
    ).set_default(-1);
    int& threads = kwarg(
        "t,threads", "Number of threads to use. [override configs, 0 to use all available cores]"
    ).set_default(-1);
    std::string& log_file = kwarg(
        "l,log-file", "Path to the log file. [optional, not working now]"
    ).set_default("");
    int& run_number = kwarg(
        "n,run-number", "Run number. [optional]"
    ).set_default(0);

    void welcome() override {
        std::cout << "Simulation using Geant4" << std::endl;
    }

    int run() override {
        this->print();
        logger->info("SimREX: Simulation starts.");

        SimREX::Simulation::run_simulation(
            this->config_path, this->beam_on, this->random_seed, this->threads, this->log_file, this->run_number
        );
        return 0;
    }
};

struct RecArgs final : argparse::Args {};

struct Arguments final : argparse::Args {
    SimArgs& sim = subcommand("simulation");
    RecArgs& rec = subcommand("reconstruction");

    bool& version = flag("v,version", "Print version");

    void welcome() override {
        logger->info("SimREX: {0}", SIMREX_VERSION);
        std::cout << std::format("SimREX [{0}]: Simulation and Reconstruction", SIMREX_VERSION) << std::endl;
    }
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<Arguments>(argc, argv);

    if (args.version) {
        std::cout << "SimREX version " << SIMREX_VERSION << std::endl;
        return 0;
    }

    args.run_subcommands();

    return 0;
}
