//
// Created by Yulei on 2024/3/7.
//

#ifndef SIMREX_HIT_H
#define SIMREX_HIT_H

// STL dependencies
#include <vector>

// ROOT dependencies
#include "TObject.h"

// Global-Event-Model dependencies
#include "global-event-model/particle.h"

using std::vector;

namespace SimREX::GEM {

    class particle;

    class particle_state;

    enum class stage : uint16_t {
        none,
        simulated,
        reconstructed,
    };

    enum class detector : uint16_t {
        none,
        tracker,
        calorimeter,
    };

    class hit : public TObject {
    public:
        hit() = default;

        ~hit() override;

        hit(stage stage, detector detector) : _stage(stage), _detector(detector) {}

        stage getStage() const {
            return _stage;
        }

        void setStage(stage stage) {
            _stage = stage;
        }

        detector getDetector() const {
            return _detector;
        }

        void setDetector(detector detector) {
            _detector = detector;
        }

        int getId() const {
            return _id;
        }

        void setId(int id) {
            _id = id;
        }

        int getCellId() const {
            return _cell_id;
        }

        void setCellId(int cellId) {
            _cell_id = cellId;
        }

        int getCellIdX() const {
            return _cell_id_x;
        }

        void setCellIdX(int cellIdX) {
            _cell_id_x = cellIdX;
        }

        int getCellIdY() const {
            return _cell_id_y;
        }

        void setCellIdY(int cellIdY) {
            _cell_id_y = cellIdY;
        }

        int getCellIdZ() const {
            return _cell_id_z;
        }

        void setCellIdZ(int cellIdZ) {
            _cell_id_z = cellIdZ;
        }

        double getX() const {
            return _x;
        }

        void setX(double x) {
            _x = x;
        }

        double getY() const {
            return _y;
        }

        void setY(double y) {
            _y = y;
        }

        double getZ() const {
            return _z;
        }

        void setZ(double z) {
            _z = z;
        }

        [[nodiscard]] const vector<double> &getT() const {
            return _t;
        }

        void setT(const vector<double> &t) {
            _t = t;
        }

        [[nodiscard]] const vector<double> &getE() const {
            return _e;
        }

        void setE(const vector<double> &e) {
            _e = e;
        }

        [[nodiscard]] const vector<particle_state *> &getParticleContributions() const {
            return _particle_contributions;
        }

        void addParticleContribution(particle_state *state);

    private:

        /*
         * Memory alignment: https://en.cppreference.com/w/cpp/language/alignas
         *
         * 00 to FF in hex corresponds to 0 to 255 in decimal.
         *
         * 00 00 00 00   00 00 00 00   00 00 00 00   00 00 00 00 --> TObject Memory
         */

        stage _stage = stage::none;      // 2 bytes
        detector _detector = detector::none; // 2 bytes

        // Integers are typically 4 bytes each. Grouping them together helps maintain alignment.
        int _id = -1;                  // 4 bytes
        int _cell_id = -1;             // 4 bytes
        int _cell_id_x = -1;           // 4 bytes
        int _cell_id_y = -1;           // 4 bytes
        int _cell_id_z = -1;           // 4 bytes

        // Doubles are typically 8 bytes each. Grouping them helps maintain 8-byte alignment.
        double _x = 0;                 // 8 bytes
        double _y = 0;                 // 8 bytes
        double _z = 0;                 // 8 bytes

        // The detailed hit time and energy change with the number of particles contributing to the hit.
        vector<double> _t = {};
        vector<double> _e = {};

        // The particles contributing to the hit.
        // each state should be pointing to the corresponding particle's _states variable
        vector<particle_state *> _particle_contributions = {};

        ClassDefOverride(hit,
        1);
    };
}


#endif //SIMREX_HIT_H
