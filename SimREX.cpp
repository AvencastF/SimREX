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
    std::string &config_path = arg("Path to the configuration file.");


    void welcome() override {
        std::cout << "Simulation using Geant4" << std::endl;
    }

    int run() override {
        run_simulation();
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
        std::cout << "SimREX: Simulation and Reconstruction" << std::endl;
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