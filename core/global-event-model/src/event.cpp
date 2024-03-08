//
// Created by Yulei on 2024/3/8.
//

#include "global-event-model/event.h"
#include "global-event-model/logger.h"

SimREX::GEM::event::event() {
    _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger("Event");
}

SimREX::GEM::event::~event() {
    initialization();
}

void SimREX::GEM::event::initialization() {
    _logger->info("Event {0} initialized", _event_number);

    _run_number = 0;
    _event_number = 0;
    _random_number = {0, 0, 0, 0};
    _event_weight = 1.0;

    // Clean MC particles
    for (auto &mcp: _mc_particles) {
        delete mcp;
    }
    _mc_particles.clear();

    // Clean Hit collections (Only clean hits in the collections, not the collections themselves)
    for (auto &hc: _hit_collections) {
        for (auto &h: hc.second) {
            delete h;
        }
        hc.second.clear();
    }

}

void SimREX::GEM::event::registerHitCollection(const TString &col_name) {
    if (_hit_collections.find(col_name) != _hit_collections.end()) {
        _logger->warn("Hit collection {0} already exists", col_name.Data());
        return;
    }

    _hit_collections[col_name] = vector<hit *>();
    _logger->info("Hit collection {0} registered", col_name.Data());
}

void SimREX::GEM::event::removeHitCollection(const TString &col_name) {
    if (_hit_collections.find(col_name) == _hit_collections.end()) {
        _logger->warn("Hit collection {0} does not exist", col_name.Data());
        return;
    }

    // Remove all hits in the collection
    for (auto &h: _hit_collections[col_name]) {
        delete h;
    }
    _hit_collections[col_name].clear();
    _hit_collections.erase(col_name);
    _logger->info("Hit collection {0} removed", col_name.Data());
}

vector<SimREX::GEM::hit *> *SimREX::GEM::event::getHits(const TString &col_name) {
    if (_hit_collections.find(col_name) == _hit_collections.end()) {
        _logger->warn("Hit collection {0} does not exist", col_name.Data());
        return nullptr;
    }

    return &_hit_collections[col_name];
}

SimREX::GEM::particle *SimREX::GEM::event::searchParticle(int id) {
    for (auto &p: _mc_particles) {
        if (p->getId() == id) {
            return p;
        }
    }
    return nullptr;
}
