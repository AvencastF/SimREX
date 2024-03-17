//
// Created by Yulei on 2024/3/14.
//

#include "reconstruction.h"

// Core dependencies
#include "global-event-model/event.h"

#include "reconstruction/sim.h"

// ROOT dependencies
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <ROOT/RDFHelpers.hxx>
#include <TSystem.h>


namespace SimREX::Reconstruction
{
    void run_reconstruction(
        const std::string& _config_path, int _random_seed, int _threads, const std::string& _log_file
    ) {
        auto chain = new TChain("sim");
        chain->Add("~/CLionProjects/SimREX/workspace/sim.root.*");

        sim t(chain);
        t.Loop();
    };
}
