//
// Created by Yulei on 2024/3/10.
//

#include "control/data_manager.h"

#include <G4VProcess.hh>


namespace SimREX::Simulation {
    data_manager::data_manager(std::string output_file_name, std::string output_tree_name, int thread_id)
        :
        _output_file_name(std::move(output_file_name)),
        _output_tree_name(std::move(output_tree_name)),
        _thread_id(thread_id) {
        _logger = GEM::LoggerManager::getInstance()->createLogger(
            std::format("Data Manager: {}", thread_id)
        );

        _event = new GEM::event();

        _output_file_name = fmt::format("{}.{}", _output_file_name, _thread_id);
    }

    data_manager::~data_manager() {
        delete _output_file;
        delete _event;
    }

    void data_manager::initialize() const {
        _event->initialize();
    }

    void data_manager::setEventInfo(int run_number, int event_number, const std::array<int, 4>& random_number) {
        _run_number = run_number;
        _event_number = event_number;

        _event->setRunNumber(_run_number);
        _event->setEventNumber(_event_number);
        _event->setRandomNumber(random_number);
    }

    void data_manager::book() {
        _logger->info("Booking output file: {} with tree: {}", _output_file_name, _output_tree_name);

        _output_file = new TFile(_output_file_name.c_str(), "RECREATE");
        if (_output_file->IsZombie()) {
            _logger->error("Failed to create output file: {}", _output_file_name);
            exit(EXIT_FAILURE);
        }
        _output_tree = new TTree(_output_tree_name.c_str(), "SimREX Event Tree");

        _output_tree->Branch("run_number", &_run_number, "run_number/I");
        _output_tree->Branch("event_number", &_event_number, "event_number/I");
        _output_tree->Branch("event", "SimREX::GEM::event", &_event, 320000000);
    }

    void data_manager::bookHitCollection(const std::string& col_name) const {
        _logger->info("Booking hit collection: {}", col_name);

        _event->registerHitCollection(col_name);
    }

    void data_manager::fillParticles() {}

    void data_manager::fill() {
        if (_output_file && _output_tree) {
            _output_tree->Fill();

            _logger->info("Event: {} filled", _event_number);

            // Print MC particles for test usage
            for (const auto& particle : *_event->getMCParticles()) {
                _logger->info(
                    "Particle: {} with PDG: {}, State size: {}, Ek = {:.3f} MeV, P = {:.3f} MeV/c",
                    particle->getId(), particle->getPdg(), particle->getStates().size(),
                    (particle->getMomentum().e() - particle->getMomentum().mass()) / CLHEP::MeV,
                    particle->getMomentum().P() / CLHEP::MeV
                );
            }
        }
    }

    void data_manager::save() {
        _output_file->cd();
        _output_tree->Write("", TObject::kOverwrite);
        _output_file->Close();

        _logger->info("Output file: {} saved", _output_file_name);
    }

    void data_manager::checkEventFilters(const G4Event* event) {}

    void data_manager::checkTrackFilters(const G4Track* track) {
        // record_all has the highest priority
        if (db::Instance()->get<bool>("truth_filter/record_all")) {
            _track_flags.record = true;
            return;
        }

        // Filter on PDG
        auto pdg_vec = db::Instance()->get<std::vector<int>>("truth_filter/PDG_applied_to_selections");
        if (
            const auto pdg = track->GetParticleDefinition()->GetPDGEncoding();
            std::ranges::find(pdg_vec, pdg) != pdg_vec.end()
        ) {
            // Filter on Kinetic Energy
            const auto energy = track->GetKineticEnergy();
            _track_flags.record = energy >= db::Instance()->get<double>("truth_filter/e_kin_min_record");
        }
    }

    void data_manager::checkStepFilters(const G4Step* step) {
        _track_flags.step = _track_flags.record;
        if (db::Instance()->get<bool>("truth_filter/record_spotless")) {
            // only for the first step
            _track_flags.step = false;
            // for the last step, it should be added in the post tracking action
        }
    }
}
