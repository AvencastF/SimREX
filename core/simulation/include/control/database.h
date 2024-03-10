//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_CONTROL_H
#define SIMREX_CONTROL_H

// core dependencies
#include "global-event-model/logger.h"
#include "global-event-model/physics_process_def.h"
#include "control/data_manager.h"

// yaml-cpp
#include <yaml-cpp/yaml.h>

// STL dependencies
#include <string>
#include <utility>
#include <vector>
#include <tuple>
#include <map>

// Geant4 dependencies
#include <G4Material.hh>
#include <G4UnitsTable.hh>
#include <G4NistManager.hh>
#include <G4UImanager.hh>

namespace SimREX::Simulation {
    using VarType = std::variant<bool, int, double, std::string, std::vector<int>, std::vector<double>>;

    class data_manager;

    class db {
    public:
        // Provide global access to the single instance of ControlClass.
        static db* Instance() {
            static db instance; // Guaranteed to be destroyed and instantiated on first use.
            return &instance;
        }

        ~db();

        // Delete copy constructor and assignment operator to prevent multiple instances.
        db(const db&) = delete;

        db& operator=(const db&) = delete;

        bool readYAML(const std::string& file_in);

        void postReadYAML();

        void readAndSetGPS();

        void readAndSetOutput();

        void printData();

        //! set variable to the core data
        void set(const std::string& name, VarType value) {
            _data[name] = std::move(value);
        }

        //! get variable from the core data, the data type should be specified
        template <typename T>
        T get(const std::string& key) {
            const auto& it = _data.find(key);
            if (it != _data.end()) {
                if (auto val = std::get_if<T>(&it->second)) {
                    return *val; // Return the value directly if found and type matches
                }
                else {
                    _logger->error("Type mismatch for key: {0}", key);
                    exit(EXIT_FAILURE);
                }
            }
            _logger->error("Key not found: {0}", key);
            exit(EXIT_FAILURE);
        }

        void processNode(const YAML::Node& node, const std::string& name, const std::map<std::string, VarType>& data);

        void registerDataManagers(int thread_id);
        data_manager* getDataManager(int thread_id);

        std::unordered_map<string, int>& getProcessMap() {
            return _process_map;
        }

    private:
        // Private constructor to prevent instantiation outside getInstance().
        db();

        void buildMaterialTable(const std::string& mat_file);

        template <typename T>
        void assign(const YAML::Node& node,
                    const std::string& name,
                    VarType& variable,
                    T default_value,
                    const bool required = false,
                    const std::string& method = ""
        ) {

            if (node[name].IsDefined()) {
                if (method.empty())
                    variable = node[name].as<T>();
                else if (method == "unit2") {
                    variable = node[name][0].as<double>() * G4UnitDefinition::GetValueOf(node[name][1].as<std::string>());
                }
                else {
                    _logger->error("Unknown method: {0}", method);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                if (required) {
                    _logger->error("Node {0} not found in YAML file, exiting", name);
                    exit(EXIT_FAILURE);
                }
                variable = default_value;
                _logger->error("Node {0} not found in YAML file, using default", name);
            }
        };

        YAML::Node _node;

        std::shared_ptr<spdlog::logger> _logger;

        std::unordered_map<std::string, VarType> _data;

        // a map of data managers, indexed by thread id
        std::unordered_map<int, data_manager*> _data_managers;
        std::mutex _data_managers_mutex;

        // a map of physics process
        std::unordered_map<string, int> _process_map;
    };
}
#endif //SIMREX_CONTROL_H
