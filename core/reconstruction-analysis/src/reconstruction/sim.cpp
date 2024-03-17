#define sim_cxx

#include "reconstruction/sim.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void sim::Loop() {
    if (fChain == nullptr) return;

    const auto output_file = new TFile("output.root", "RECREATE");
    const auto output_tree = new TTree("rec", "SimREX Event Tree - reconstructed");

    output_tree->Branch("run_number", &run_number, "run_number/I");
    output_tree->Branch("event_number", &event_number, "event_number/I");

    bool is_decay = false;
    output_tree->Branch("is_decay", &is_decay, "is_decay/O");

    double decay_E = 0;
    double decay_x = 0;
    double decay_y = 0;
    double decay_z = 0;
    output_tree->Branch("decay_E", &decay_E, "decay_E/D");
    output_tree->Branch("decay_x", &decay_x, "decay_x/D");
    output_tree->Branch("decay_y", &decay_y, "decay_y/D");
    output_tree->Branch("decay_z", &decay_z, "decay_z/D");


    Long64_t n_entries = fChain->GetEntriesFast();

    Long64_t n_bytes = 0, nb = 0;
    for (Long64_t j_entry = 0; j_entry < n_entries; j_entry++) {
        Long64_t i_entry = LoadTree(j_entry);
        if (i_entry < 0) break;
        nb = fChain->GetEntry(j_entry);
        n_bytes += nb;

        is_decay = false;
        decay_E = 0;
        decay_x = 0;
        decay_y = 0;
        decay_z = 0;
        // Find the initial particles
        for (const auto mc_particles = event->getMCParticles(); const auto& particle : *mc_particles) {
            if (particle->getDepth() == 0) {
                // Get Particle States
                const auto particle_states = particle->getStates();
                for (const auto& state : particle_states) {
                    if (event->getProcessName(state.process_id) == "Decay") {
                        is_decay = true;

                        decay_x = state.position.x();
                        decay_y = state.position.y();
                        decay_z = state.position.z();
                        decay_E = state.momentum.energy() - state.momentum.mass();
                    }
                }
            }
        }

        output_tree->Fill();
    }

    output_file->cd();
    output_tree->Write("", TObject::kOverwrite);
    output_file->Close();
}
