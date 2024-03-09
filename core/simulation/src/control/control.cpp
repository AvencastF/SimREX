//
// Created by Yulei on 2024/3/9.
//

#include "control/control.h"

#include "G4SystemofUnits.hh"

namespace SimREX::Simulation {


    control::control() {
        _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("control");
    }

    bool control::readYAML(const std::string &file_in) {

        try {
            _node = YAML::LoadFile(file_in);
        } catch (YAML::BadFile &e) {
            _logger->error("File {} not found", file_in);
            return false;
        }

        /*
         * Step 1: Building Material Table
         */
        try {
            _data["material_file"] = _node["material_file"].as<std::string>(); // should be defined explicitly
        } catch (YAML::InvalidNode &e) {
            _logger->error("Error reading material file");
            exit(EXIT_FAILURE);
        }
        buildMaterialTable(std::get<std::string>(_data["material_file"]));

        /*
         * Step 2: Reading all Global Configs
         */
        assign<int>(_node, "random_seed", _data["random_seed"], 0);
        assign<int>(_node, "beam_on", _data["beam_on"], 0);

        // Verbosity
        auto node_name = "verbosity";
        if (auto sub_node = _node[node_name]; sub_node.IsDefined()) {
            assign<int>(sub_node, "run", _data["verbosity/run"], 2);
            assign<int>(sub_node, "event", _data["verbosity/event"], 0);
            assign<int>(sub_node, "tracking", _data["verbosity/tracking"], 0);
        } else {
            _logger->warn("Verbosity not defined, using default values");
            _data["verbosity/run"] = 2;
            _data["verbosity/event"] = 0;
            _data["verbosity/tracking"] = 0;
        }

        /*
         * Step 3: Reading General Particle Source
         */
//        readAndSetGPS();


        return true;
    }

    void control::buildMaterialTable(const std::string &mat_file) {

        auto readMatDensity = [this](const YAML::Node &node) -> double {
            if (node.size() != 3) {
                this->_logger->error("Value size is incompatible with unit: [density, weight unit, length unit]");
                exit(EXIT_FAILURE);
            }
            if (!node[0].IsScalar()) {
                _logger->error("Node ({}) is not defined or not a number", node[0].as<std::string>());
                exit(EXIT_FAILURE);
            }
            return
                    node[0].as<double>()
                    * G4UnitDefinition::GetValueOf(node[1].as<std::string>())
                    / G4UnitDefinition::GetValueOf(node[2].as<std::string>());
        };

        YAML::Node mat_node;
        try {
            mat_node = YAML::LoadFile(mat_file);
        } catch (YAML::BadFile &e) {
            _logger->error("Error reading material file {0}: {1}", mat_file, e.msg);
            exit(EXIT_FAILURE);
        }
        _logger->info("Material file {0} read successfully", mat_file);

        /*
         * For external material
         */
        try {
            auto n = mat_node["build_material"];
            /* For a material definition,
             * 1. Name (Same as material string)
             * 2. Component, must be 2 * (# of element), defined as ["Element Name", percentage]
             * 3. Density
             * 4. state: solid or gas or liquid
             */
            for (auto mat: n) {
                auto mat_name = mat["name"].as<std::string>();
                auto mat_den = readMatDensity(mat["density"]);
                G4State mat_state;
                if (auto state = mat["state"].as<std::string>(); state == "gas") mat_state = kStateGas;
                else if (state == "liquid") mat_state = kStateLiquid;
                else if (state == "solid") mat_state = kStateSolid;
                else {
                    _logger->error("Error defining material state: {0}", state);
                    exit(EXIT_FAILURE);
                }

                int n_com = static_cast<int>(mat["composition"].size());
                if (n_com % 2 != 0) {
                    _logger->error("Error defining material composition: {0}", mat_name);
                    exit(EXIT_FAILURE);
                }
                auto new_mat = new G4Material(mat_name, mat_den, n_com / 2, mat_state);

                std::string comp_str;
                for (size_t i = 0; i < mat["composition"].size(); i += 2) {
                    new_mat->AddElement(
                            G4NistManager::Instance()->FindOrBuildElement(mat["composition"][i].as<std::string>()),
                            mat["composition"][i + 1].as<double>()
                    );
                    comp_str += fmt::format(" {0}: {1:.3g}", mat["composition"][i].as<std::string>(),
                                            mat["composition"][i + 1].as<double>());
                }

                _logger->info(
                        "Material {0} \n\t\t density: {1:.5g} g/cm^3 \n\t\t composition: {2}",
                        mat_name, mat_den / g * cm3, comp_str
                );
            }
        }
        catch (YAML::InvalidNode &e) {
            _logger->error("Error reading material file: {}", e.msg);
            exit(EXIT_FAILURE);
        }

        /*
         * For predefined material
         */
        try {
            auto n = mat_node["internal_material"];
            for (auto mat: n) {
                G4NistManager::Instance()->FindOrBuildMaterial(mat["name"].as<std::string>());
                _logger->info("Material built --> {0} ", mat["name"].as<std::string>());
            }
        }
        catch (YAML::InvalidNode &e) {
            _logger->error("Error reading material file: {}", e.msg);
            exit(EXIT_FAILURE);
        }
    }

    void control::readAndSetGPS() {
        auto node_name = "general_particle_source";
        auto UIManager = G4UImanager::GetUIpointer();
        if (auto gps_node = _node[node_name]; gps_node.IsDefined()) {
            for (auto cmd: gps_node) {
                printf(
                        "GPS settings: /gps/%s %s \n", cmd.first.as<std::string>().data(),
                        cmd.second.as<std::string>().data()
                );
                UIManager->ApplyCommand("/gps/" + cmd.first.as<std::string>() + " " +
                                        cmd.second.as<std::string>());
            }
        }
    }


    void control::printData() {
        std::string detail;
        for (const auto &[key, value]: _data) {
            std::visit([key, &detail](auto &&arg) {
                detail += std::format("\t{0:>40} : {1}\n", key, arg);
            }, value);
        }
        _logger->info("Control Data: \n{0}", detail);
    }
}