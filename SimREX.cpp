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
auto logger = LoggerManager->createLogger("0-SimREX");

struct SimArgs : public argparse::Args {
    std::string &config_path = arg("config_path", "Path to the configuration file. [yaml file]");
    unsigned &beam_on = kwarg(
            "b,beam-on", "Number of beam-on events. [override configs]",/*implicit*/"100"
    ).set_default(100);
    unsigned &random_seed = kwarg(
            "r,random-seed", "Random seed for whole simulation. [override configs]",/*implicit*/"0"
    ).set_default(0);

    void welcome() override {
        std::cout << "Simulation using Geant4" << std::endl;
    }

    int run() override {
        this->print();
        logger->info("SimREX: Simulation starts.");

        SimREX::Simulation::run_simulation();
        return 0;
    }
};

struct RecArgs : public argparse::Args {
};

struct Arguments : public argparse::Args {
    SimArgs &sim = subcommand("simulation");
    RecArgs &rec = subcommand("reconstruction");

    bool &version = flag("v,version", "Print version");

    void welcome() override {
        logger->info("SimREX: {0}", SIMREX_VERSION);
        std::cout << std::format("SimREX [{0}]: Simulation and Reconstruction", SIMREX_VERSION) << std::endl;
    }
};

int main(int argc, char *argv[]) {


    auto args = argparse::parse<Arguments>(argc, argv);

    if (args.version) {
        std::cout << "SimREX version " << SIMREX_VERSION << std::endl;
        return 0;
    }

    args.run_subcommands();

    return 0;
}