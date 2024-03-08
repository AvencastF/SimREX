//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_PARTICLE_H
#define SIMREX_PARTICLE_H

// STL dependencies
#include <vector>

// ROOT dependencies
#include "TObject.h"
#include "Math/Vector4D.h"

// Global-Event-Model dependencies
#include "global-event-model/hit.h"

using std::vector;
using position_t = ROOT::Math::XYZTVector;
using momentum_E = ROOT::Math::PxPyPzEVector;

namespace SimREX::GEM {

    class hit;

    class particle;

    struct particle_state {
        int process_id = -1; // unique id for this state
        double dE = 0; // energy change in this state
        hit *hit = nullptr;
        particle *particle = nullptr;
        position_t position = {0, 0, 0, -1};
        momentum_E momentum = {0, 0, 0, 0};

        ~particle_state() {
            if (particle) {
                particle = nullptr;
            }
            if (hit) {
                hit = nullptr;
            }
        };
    };

    class particle : public TObject {
    public:

        particle() = default;

        ~particle() override;

        int getId() const {
            return _id;
        }

        int getPdg() const {
            return _pdg;
        }

        int getCharge() const {
            return _charge;
        }

        void setId(int id) {
            _id = id;
        }

        void setPdg(int pdg) {
            _pdg = pdg;
        }

        void setCharge(int charge) {
            _charge = charge;
        }

        int getDepth() const {
            return _depth;
        }

        void setDepth(int depth) {
            _depth = depth;
        }

        [[nodiscard]] const position_t &getVertex() const {
            return _vertex;
        }

        [[nodiscard]] const position_t &getEndVertex() const {
            return _end_vertex;
        }

        [[nodiscard]] const momentum_E &getMomentum() const {
            return _momentum;
        }

        void setVertex(const position_t &vertex) {
            _vertex = vertex;
        }

        void setEndVertex(const position_t &endVertex) {
            _end_vertex = endVertex;
        }

        void setMomentum(const momentum_E &momentum) {
            _momentum = momentum;
        }

        void setVertex(double x, double y, double z, double t) {
            _vertex = {x, y, z, t};
        }

        void setEndVertex(double x, double y, double z, double t) {
            _end_vertex = {x, y, z, t};
        }

        void setMomentum(double px, double py, double pz, double e) {
            _momentum = {px, py, pz, e};
        }

        [[nodiscard]] particle *getParent() const {
            return _parent;
        }

        void setParent(particle *parent) {
            _parent = parent;
        }

        [[nodiscard]] const vector<particle *> &getChildren() const {
            return _children;
        }

        void setChildren(const vector<particle *> &children) {
            _children = children;
        }

        [[nodiscard]] const vector<particle_state> &getStates() const {
            return _states;
        }

        void addChild(particle *child);

        void removeChild(particle *child);

        void addState(particle_state state);

    private:
        // Track ID (unique for each track in the event)
        int _id = -1;
        // Depth of the particle to the primary particle in the event
        int _depth = -1;
        int _pdg = 0;
        int _charge = 0;

        particle *_parent = nullptr;
        vector<particle *> _children;

        /*
         * Physics Vectors by ROOT: https://root.cern.ch/doc/master/group__GenVector.html
         */

        position_t _vertex = {0, 0, 0, -1};
        position_t _end_vertex = {0, 0, 0, -1};
        momentum_E _momentum = {0, 0, 0, 0};

        vector<particle_state> _states = {};

    ClassDefOverride(particle, 1);
    };
}

#endif //SIMREX_PARTICLE_H
