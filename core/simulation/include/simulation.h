//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_SIMULATION_H
#define SIMREX_SIMULATION_H

#include <string>

namespace SimREX::Simulation {

    int run_simulation(const std::string& _config_path, int _beam_on, int _random_seed, int _threads);

}

#endif //SIMREX_SIMULATION_H