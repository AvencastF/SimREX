//
// Created by Yulei on 2024/3/9.
//

#include "control/control.h"

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

        // Read the material file
        std::string material_file;
        try {
            material_file = _node["material_file"].as<std::string>(); // should be defined explicitly
        } catch (YAML::InvalidNode &e) {
            _logger->error("Error reading material file");
            exit(EXIT_FAILURE);
        }
        buildMaterialTable(material_file);

        return true;
    }

    void control::buildMaterialTable(const std::string &mat_file) {

    }
}