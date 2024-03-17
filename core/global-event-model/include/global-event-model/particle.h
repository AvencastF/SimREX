//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_PARTICLE_H
#define SIMREX_PARTICLE_H

// STL dependencies
#include <vector>

// ROOT dependencies
#include <TObject.h>
#include <Math/Vector4D.h>

// Global-Event-Model dependencies
#include "global-event-model/hit.h"

using std::vector;
using position_t = ROOT::Math::XYZTVector;
using momentum_E = ROOT::Math::PxPyPzEVector;

namespace SimREX::GEM {

    class hit;

    class particle;

    /**
 * @struct particle_state
 * @brief This struct represents the state of a particle at a given point in time.
 *
 * The `particle_state` struct is a data structure that stores information about a particle's state.
 * It inherits from the TObject class, which is the base class for all objects in the ROOT system.
 *
 * A `particle_state` object contains the following information:
 * - `process_id`: The ID of the process according to physics_process_def.h.
 * - `dE`: The energy change in this state.
 * - `hit`: A pointer to the hit object associated with this state.
 * - `particle`: A pointer to the particle object associated with this state.
 * - `position`: The position of the particle in this state, represented as a 4D vector (x, y, z, t).
 * - `momentum`: The momentum of the particle in this state, represented as a 4D vector (px, py, pz, E).
 *
 * The struct provides a destructor to ensure that the pointers to the particle and hit objects are set to nullptr when the `particle_state` object is destroyed.
 *
 * @note The position and momentum are represented using ROOT's Math::XYZTVector and Math::PxPyPzEVector classes, respectively.
 */
    struct particle_state final : TObject {
        int process_id = -1; // process ID according to physics_process_def.h
        double dE = 0; // energy change in this state
        hit* hit = nullptr;
        particle* particle = nullptr;
        position_t position = {0, 0, 0, -1};
        momentum_E momentum = {0, 0, 0, 0};

        ~particle_state() override {
            if (particle) {
                particle = nullptr;
            }
            if (hit) {
                hit = nullptr;
            }
        };
        ClassDefOverride(particle_state, 1);
    };

    class particle final : public TObject {
    public:
        particle() = default;

        ~particle() override;

        int getId() const {
            return _id;
        }

        int getPdg() const {
            return _pdg;
        }

        double getCharge() const {
            return _charge;
        }

        void setId(int id) {
            _id = id;
        }

        void setPdg(int pdg) {
            _pdg = pdg;
        }

        void setCharge(double charge) {
            _charge = charge;
        }

        int getDepth() const {
            return _depth;
        }

        void setDepth(int depth) {
            _depth = depth;
        }

        [[nodiscard]] particle* getParent() const {
            return _parent;
        }

        void setParent(particle* parent) {
            _parent = parent;
            _depth = parent->getDepth() + 1;
        }

        [[nodiscard]] const vector<particle*>& getChildren() const {
            return _children;
        }

        void setChildren(const vector<particle*>& children) {
            _children = children;
        }

        [[nodiscard]] const vector<particle_state>& getStates() const {
            return _states;
        }

        [[nodiscard]] momentum_E getMomentum() const {
            return _states.front().momentum;
        }

        [[nodiscard]] position_t getVertex() const {
            return _states.front().position;
        }

        [[nodiscard]] position_t getEndVertex() const {
            return _states.back().position;
        }


        void addChild(particle* child);

        void removeChild(particle* child);

        void addState(particle_state state);

        void addHitCache(hit* _hit) {
            if (!_hit_cache) {
                _hit_cache = _hit;
            }
            else {
                std::cerr << "Hit cache already exists" << std::endl;
                _hit_cache = nullptr;
            }
        }

        void processHitCache();

    private:
        // Track ID (unique for each track in the event)
        int _id = -1;
        // Depth of the particle to the primary particle in the event
        int _depth = -1;
        int _pdg = 0;
        double _charge = 0;

        particle* _parent = nullptr;
        vector<particle*> _children;

        /*
         * Physics Vectors by ROOT: https://root.cern.ch/doc/master/group__GenVector.html
         */

        vector<particle_state> _states = {};

        hit* _hit_cache = nullptr; //!

        ClassDefOverride(particle, 1);
    };
}

#endif //SIMREX_PARTICLE_H
