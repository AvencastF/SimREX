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

    struct particle_state;

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

    /**
     * @class hit
     * @brief This class represents a hit in a detector.
     *
     * The `hit` class is a data structure that stores information about a hit in a detector.
     * It inherits from the TObject class, which is the base class for all objects in the ROOT system.
     *
     * A `hit` object contains the following information:
     * - `_stage`: The stage where the hit occurred. It can be none, simulated, or reconstructed.
     * - `_detector`: The detector where the hit occurred. It can be none, tracker, or calorimeter.
     * - `_id`: The ID of the hit. (for debug)
     * - `_cell_id`: The ID of the cell where the hit occurred.
     * - `_cell_id_x`: The x-coordinate of the cell where the hit occurred.
     * - `_cell_id_y`: The y-coordinate of the cell where the hit occurred.
     * - `_cell_id_z`: The z-coordinate of the cell where the hit occurred.
     * - `_x`: The x-coordinate of the hit.
     * - `_y`: The y-coordinate of the hit.
     * - `_z`: The z-coordinate of the hit.
     * - `_t`: The time of the hit. It changes with the number of particles contributing to the hit.
     * - `_e`: The energy of the hit. It changes with the number of particles contributing to the hit.
     * - `_particle_contributions`: The particles that contributed to the hit. Each particle state should be pointing to the corresponding particle's _states variable.
     *
     * The class provides getter and setter methods for all its data members. It also provides a method
     * to add a particle contribution to the hit.
     *
     * @note `_particle_contributions` is empty after simulation, which needs to be linked in the reconstruction. Find hits in the particle states and link them to the hit.
     * @note The cell ID for tracker hit is the layer
     * @note The cell ID z for tracker hit is the sub-layer
     */
    class hit final : public TObject {
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

        [[nodiscard]] vector<double>& getT() {
            return _t;
        }

        void setT(const vector<double>& t) {
            _t = t;
        }

        [[nodiscard]] vector<double>& getE() {
            return _e;
        }

        void setE(const vector<double>& e) {
            _e = e;
        }

        [[nodiscard]] const vector<particle_state*>& getParticleContributions() const {
            return _particle_contributions;
        }

        void addParticleContribution(particle_state* state);

    private:
        /*
         * Memory alignment: https://en.cppreference.com/w/cpp/language/alignas
         *
         * 00 to FF in hex corresponds to 0 to 255 in decimal.
         *
         * 00 00 00 00   00 00 00 00   00 00 00 00   00 00 00 00 --> TObject Memory
         */

        stage _stage = stage::none; // 2 bytes
        detector _detector = detector::none; // 2 bytes

        // Integers are typically 4 bytes each. Grouping them together helps maintain alignment.
        int _id = -1; // 4 bytes
        int _cell_id = -1; // 4 bytes
        int _cell_id_x = -1; // 4 bytes
        int _cell_id_y = -1; // 4 bytes
        int _cell_id_z = -1; // 4 bytes

        // Doubles are typically 8 bytes each. Grouping them helps maintain 8-byte alignment.
        double _x = 0; // 8 bytes
        double _y = 0; // 8 bytes
        double _z = 0; // 8 bytes

        // The detailed hit time and energy change with the number of particles contributing to the hit.
        vector<double> _t = {};
        vector<double> _e = {};

        // The particles contributing to the hit.
        // each state should be pointing to the corresponding particle's _states variable
        vector<particle_state*> _particle_contributions = {};

        ClassDefOverride(hit, 1);
    };
}


#endif //SIMREX_HIT_H
