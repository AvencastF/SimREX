//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_CONTROL_H
#define SIMREX_CONTROL_H

// yaml-cpp
#include "yaml-cpp/yaml.h"

// STL dependencies
#include <string>
#include <vector>
#include <tuple>
#include <map>


namespace SimREX::Simulation {
    class control {
    public:
        // Provide global access to the single instance of ControlClass.
        static control *getInstance() {
            static control instance;  // Guaranteed to be destroyed and instantiated on first use.
            return &instance;
        }

        // Delete copy constructor and assignment operator to prevent multiple instances.
        control(const control &) = delete;

        control &operator=(const control &) = delete;

        bool ReadYAML(const std::string &file_in);

    private:
        // Private constructor to prevent instantiation outside getInstance().
        control();

    };
}

#endif //SIMREX_CONTROL_H
