//
// Created by Yulei on 2024/3/9.
//

#include "control/database.h"

#include "G4SystemofUnits.hh"

namespace SimREX::Simulation {
    db::db() {
        _logger = GEM::LoggerManager::getInstance()->createLogger("control");

        _process_map = GEM::PhysicsDef().dPhyTypeMap;
    }

    db::~db() {
        for (auto& [key, value] : _data_managers) {
            delete value;
        }
    }

    bool db::readYAML(const std::string& file_in) {
        try {
            _node = YAML::LoadFile(file_in);
        }
        catch (YAML::BadFile&) {
            _logger->error("File {} not found", file_in);
            return false;
        }

        /*
         * Step 1: Building Material Table
         */
        if (!_node["material_file"].IsDefined()) {
            buildMaterialTable("", &_node);
        }
        else {
            _data["material_file"] = _node["material_file"].as<std::string>();
            buildMaterialTable(std::get<std::string>(_data["material_file"]));
        }
        /*
         * Step 2: Reading all Global Configs
         */
        assign<int>(_node, "random_seed", _data["random_seed"], 0);
        assign<int>(_node, "beam_on", _data["beam_on"], 0);
        assign<int>(_node, "threads", _data["threads"], 0);

        // Verbosity
        // Attention: applying verbosity will be set in the action initialization for master thread.
        processNode(_node, "verbosity", {
                        {"run", 2},
                        {"event", 0},
                        {"tracking", 0},
                        {"stepping", 0},
                    });

        // Output
        readAndSetOutput();

        /*
         * Step 3: Reading General Particle Source
         *
         * Attention: this part should be set in the main program, not here.
         */


        /*
         * Step 4: Reading Detector Geometry Configs
         */
        if (!_node["detector_file"].IsDefined()) {
            buildGeometry("", &_node);
        }
        else {
            _data["detector_file"] = _node["detector_file"].as<std::string>();
            buildGeometry(std::get<std::string>(_data["detector_file"]));
        }


        return true;
    }

    void db::postReadYAML() {}

    void db::processNode(const YAML::Node& node, const std::string& name, const std::map<std::string, VarType>& data) {
        bool once = false;
        for (const auto& [key, value] : data) {
            if (const auto sub_node = node[name]; sub_node.IsDefined()) {
                std::visit([key, name, &sub_node, this](auto&& arg) {
                    std::string _key = key;
                    std::string method;
                    if (const auto pos = std::ranges::find(key, '@'); pos != key.end()) {
                        method = std::string{pos + 1, key.end()};
                        _key = std::string{key.begin(), pos};
                    }
                    using d_type = std::decay_t<decltype(arg)>;
                    if constexpr (!std::is_same_v<d_type, std::vector<tracker_region>>)
                        assign<d_type>(sub_node, _key, _data[std::format("{0}/{1}", name, _key)], arg, false, method);
                    else
                        std::cout << "Hi" << std::endl;
                }, value);
            }
            else {
                if (!once) {
                    _logger->warn("{} not defined, using default values", name);
                    once = true;
                }

                std::visit([key, name, this](auto&& arg) {
                    _data[std::format("{0}/{1}", name, key)] = arg;
                }, value);
            }
        }
    }

    void db::registerDataManagers(int thread_id) {
        std::lock_guard<std::mutex> lock(_data_managers_mutex);
        _data_managers[thread_id] = new data_manager(
            std::get<std::string>(_data["data_manager/output_file_name"]),
            std::get<std::string>(_data["data_manager/output_tree_name"]),
            thread_id
        );

        _logger->info("Data Manager for thread {} registered", thread_id);
    }

    data_manager* db::getDataManager(int thread_id) {
        if (auto it = _data_managers.find(thread_id); it != _data_managers.end()) {
            return it->second;
        }
        _logger->error("Data Manager for thread {} not found", thread_id);
        exit(EXIT_FAILURE);
    }

    void db::buildMaterialTable(const std::string& mat_file, YAML::Node* _mat_node) const {
        auto readMatDensity = [this](const YAML::Node& node) -> double {
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
        if (_mat_node != nullptr) {
            mat_node = *_mat_node;
        }
        else {
            try {
                mat_node = YAML::LoadFile(mat_file);
            }
            catch (YAML::BadFile& e) {
                _logger->error("Error reading material file {0}: {1}", mat_file, e.msg);
                exit(EXIT_FAILURE);
            }
            _logger->info("Material file {0} read successfully", mat_file);
        }
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
            for (auto mat : n) {
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
        catch (YAML::InvalidNode& e) {
            _logger->error("Error reading material file: {}", e.msg);
            exit(EXIT_FAILURE);
        }

        /*
         * For predefined material
         */
        try {
            auto n = mat_node["internal_material"];
            for (auto mat : n) {
                G4NistManager::Instance()->FindOrBuildMaterial(mat["name"].as<std::string>());
                _logger->info("Material built --> {0} ", mat["name"].as<std::string>());
            }
        }
        catch (YAML::InvalidNode& e) {
            _logger->error("Error reading material file: {}", e.msg);
            exit(EXIT_FAILURE);
        }
    }

    void db::buildGeometry(const std::string& geo_file, YAML::Node* _geo_node) {
        YAML::Node geo_node;
        if (_geo_node != nullptr) {
            geo_node = *_geo_node;
        }
        else {
            try {
                geo_node = YAML::LoadFile(geo_file);
            }
            catch (YAML::BadFile& e) {
                _logger->error("Error reading geometry file {0}: {1}", geo_file, e.msg);
                exit(EXIT_FAILURE);
            }
            _logger->info("Geometry file {0} read successfully", geo_file);
        }

        // World
        processNode(geo_node, "world", {
                        {"material", "vacuum"},
                        {"size@unit-v3", std::vector<double>{1, 1, 1}},
                    });

        // Building List
        assign<std::vector<std::string>>(geo_node, "build-list", _data["build-list"], std::vector<std::string>{}, true);

        // Tracker-like
        if (auto t_node = geo_node["tracker-like"]; t_node.IsDefined()) {
            buildTrackerLike(&t_node);
        }
    }

    void db::buildTrackerLike(YAML::Node* _t_node) {
        // Region size should be computed in the detector construction part
        std::vector<tracker_region> tracker_regions;
        for (auto _region_node : *_t_node) {
            // First reading regions
            tracker_region trk_reg;
            trk_reg.name = _region_node["name"].as<std::string>();
            trk_reg.material = _region_node["material"].as<std::string>();
            trk_reg.position = unit_v3(_region_node["position"]);

            // Second reading daughters
            for (auto _trk_node : _region_node["daughters"]) {
                tracker daughter;
                daughter.name = _trk_node["name"].as<std::string>();
                daughter.material = _trk_node["material"].as<std::string>();
                daughter.offset = unit_2(_trk_node["offset"]);

                auto read_layer = [this](const YAML::Node& _node) {
                    if (!_node.IsDefined())
                        return tracker_layer{};

                    tracker_layer layer;
                    for (size_t i = 0; i < _node.size(); i++) {
                        layer.emplace_back(unit_v3(_node[i]));
                    }
                    return layer;
                };

                daughter.position = read_layer(_trk_node["position"]);
                daughter.size = read_layer(_trk_node["size"]);
                daughter.rotation = read_layer(_trk_node["rotation"]);
                daughter.strip_per_layer = _trk_node["strip_per_layer"].as<std::vector<int>>();

                trk_reg.daughters.emplace_back(daughter);
            }
            tracker_regions.emplace_back(trk_reg);
        }
        _data["tracker-like"] = tracker_regions;
    }

    void db::readAndSetGPS() {
        auto node_name = "general_particle_source";
        auto UIManager = G4UImanager::GetUIpointer();

        if (auto gps_node = _node[node_name]; gps_node.IsDefined()) {
            std::string gps_str;
            for (auto cmd : gps_node) {
                gps_str += std::format(
                    "\t/gps/{0} {1}\n", cmd.first.as<std::string>(), cmd.second.as<std::string>()
                );
                UIManager->ApplyCommand("/gps/" + cmd.first.as<std::string>() + " " +
                    cmd.second.as<std::string>());
            }
            _logger->info("General Particle Source: \n{0}", gps_str);
        }
    }

    void db::readAndSetOutput() {
        // Data Manager Configs
        processNode(_node, "data_manager", {
                        {"output_file_name", "sim.root"},
                        {"output_tree_name", "sim"},
                    });

        // Truth Filter
        processNode(_node, "truth_filter", {
                        {"PDG_applied_to_selections", std::vector<int>{-11, 11, 22}},
                        {"e_kin_min_record@unit2", 0 * MeV},
                        {"record_all", false},
                        {"record_spotless", true},
                    });
    }

    void db::printData() {
        std::string detail;
        for (const auto& [key, value] : _data) {
            std::visit([key, &detail](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                // For vector structure
                if constexpr (
                    std::is_same_v<T, std::vector<int>> ||
                    std::is_same_v<T, std::vector<double>> ||
                    std::is_same_v<T, std::vector<std::string>>
                ) {
                    detail += std::format("\t{0:>40} : [ ", key);
                    for (const auto& i : arg) {
                        detail += std::format("{}, ", i);
                    }
                    detail += std::format("] \n");
                }
                else if constexpr (std::is_same_v<T, std::vector<tracker_region>>) {}
                else {
                    // For plain data type
                    detail += std::format("\t{0:>40} : {1}\n", key, arg);
                }
            }, value);
        }
        _logger->info("Control Data: \n{0}", detail);
    }
}
