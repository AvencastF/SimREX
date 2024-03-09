//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_CONTROL_H
#define SIMREX_CONTROL_H

// core dependencies
#include "global-event-model/logger.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

// STL dependencies
#include <string>
#include <utility>
#include <vector>
#include <tuple>
#include <map>

// Geant4 dependencies
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include "G4UImanager.hh"

namespace SimREX::Simulation {

    using VarType = std::variant<bool, int, double, std::string>;

    class control {
    public:
        // Provide global access to the single instance of ControlClass.
        static control *Instance() {
            static control instance;  // Guaranteed to be destroyed and instantiated on first use.
            return &instance;
        }

        // Delete copy constructor and assignment operator to prevent multiple instances.
        control(const control &) = delete;

        control &operator=(const control &) = delete;

        bool readYAML(const std::string &file_in);

        void readAndSetGPS();

        void printData();

        void setData(const std::string &name, VarType value) {
            _data[name] = std::move(value);
        }

        template<typename T>
        T getValue(const std::string &key) {
            auto it = _data.find(key);
            if (it != _data.end()) {
                if (auto val = std::get_if<T>(&it->second)) {
                    return *val; // Return the value directly if found and type matches
                } else {
                    _logger->error("Type mismatch for key: {0}", key);
                    exit(EXIT_FAILURE);
                }
            }
            _logger->error("Key not found: {0}", key);
            exit(EXIT_FAILURE);
        }

    private:
        // Private constructor to prevent instantiation outside getInstance().
        control();

        void buildMaterialTable(const std::string &mat_file);

        template<typename T>
        void assign(const YAML::Node &node,
                    const std::string &name,
                    VarType &variable,
                    T default_value,
                    bool required = false
        ) {
            if (node[name].IsDefined()) {
                variable = node[name].as<T>();
            } else {
                variable = default_value;
                _logger->error("Node {0} not found in YAML file, using default: {1}", name, default_value);
            }
        };

        YAML::Node _node;

        std::shared_ptr<spdlog::logger> _logger;

        std::unordered_map<std::string, VarType> _data;
    };

}
#endif //SIMREX_CONTROL_H
