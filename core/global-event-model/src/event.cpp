//
// Created by Yulei on 2024/3/8.
//

#include "global-event-model/event.h"
#include "global-event-model/logger.h"

namespace SimREX::GEM
{
    event::event() {
        _logger = LoggerManager::getInstance()->createLogger("Event");
    }

    event::~event() {
        initialize();
    }

    void event::initialize() {
#ifdef DEBUG
        _logger->info("Event {0} initialized", _event_number);
#endif
        _run_number = 0;
        _event_number = 0;
        _random_number = {0, 0, 0, 0};
        _event_weight = 1.0;

        // Clean MC particles
        for (auto& mcp : _mc_particles) {
            delete mcp;
        }
        _mc_particles.clear();

        // Clean Hit collections (Only clean hits in the collections, not the collections themselves)
        for (auto& hc : _hit_collections) {
            for (auto& h : hc.second) {
                delete h;
            }
            hc.second.clear();
        }
    }

    vector<hit*>* event::registerHitCollection(const TString& col_name) {
        if (_hit_collections.contains(col_name)) {
            _logger->warn("Hit collection {0} already exists", col_name.Data());
            return &_hit_collections[col_name];
        }

        _hit_collections[col_name] = vector<hit*>();
        _logger->info("Hit collection {0} registered", col_name.Data());
        return &_hit_collections[col_name];
    }

    void event::removeHitCollection(const TString& col_name) {
        if (!_hit_collections.contains(col_name)) {
            _logger->warn("Hit collection {0} does not exist", col_name.Data());
            return;
        }

        // Remove all hits in the collection
        for (auto& h : _hit_collections[col_name]) {
            delete h;
        }
        _hit_collections[col_name].clear();
        _hit_collections.erase(col_name);
        _logger->info("Hit collection {0} removed", col_name.Data());
    }

    vector<hit*>* event::getHits(const TString& col_name) {
        if (!_hit_collections.contains(col_name)) {
            _logger->warn("Hit collection {0} does not exist", col_name.Data());
            return nullptr;
        }

        return &_hit_collections[col_name];
    }

    particle* event::searchParticle(int id) const {
        for (auto& p : _mc_particles) {
            if (p->getId() == id) {
                return p;
            }
        }
        return nullptr;
    }

    TString event::getProcessName(int id) const {
        return {_physics_def.dPhyTypeVec.at(id)};
    }
}
