//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_SIMULATION_H
#define SIMREX_SIMULATION_H

#include <string>
#include <vector>

namespace SimREX::Simulation {
    inline std::vector<std::string> vis_commands = {
        "/vis/open",
        "/vis/viewer/set/autoRefresh false",
        "/vis/verbose errors",
        "/vis/drawVolume",
        "/vis/viewer/set/viewpointThetaPhi 90. 180.",
        "/vis/viewer/zoom 1.4",
        // Specify style (surface, wireframe, auxiliary edges,...)
        "/vis/viewer/set/style wireframe",
        "/vis/viewer/set/auxiliaryEdge true",
        "/vis/viewer/set/lineSegmentsPerCircle 100",
        // Draw smooth trajectories at end of event, showing trajectory points
        // as markers 2 pixels wide:
        "/vis/scene/add/trajectories smooth",
        "/vis/modeling/trajectories/create/drawByCharge",
        "/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true",
        "/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2",
        "/vis/scene/add/hits",
        // Re-establish auto refreshing and verbosity:
        "/vis/viewer/set/autoRefresh true",
        "/vis/verbose warnings"
    };

    void run_simulation(const std::string& _config_path, int _beam_on, int _random_seed, int _threads,
                        const std::string& _log_file, int _run_number, bool _visual);
}

#endif //SIMREX_SIMULATION_H
