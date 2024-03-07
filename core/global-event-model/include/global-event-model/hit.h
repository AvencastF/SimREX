//
// Created by Yulei on 2024/3/7.
//

#ifndef SIMREX_HIT_H
#define SIMREX_HIT_H

// STL dependencies
#include <vector>

// ROOT dependencies
#include "TObject.h"

using std::vector;

namespace SimREX::GEM {

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

        bool operator==(const GEM::hit &rhs) const {
            return
                    _stage == rhs._stage &&
                    _detector == rhs._detector &&
                    _id == rhs._id &&
                    _cell_id == rhs._cell_id &&
                    _cell_id_x == rhs._cell_id_x &&
                    _cell_id_y == rhs._cell_id_y &&
                    _cell_id_z == rhs._cell_id_z &&
                    _x == rhs._x &&
                    _y == rhs._y &&
                    _z == rhs._z &&
                    _t == rhs._t &&
                    _e == rhs._e;
        }

        bool operator!=(const GEM::hit &rhs) const {
            return !(rhs == *this);
        }

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

        vector<double> _t = {};                 // 8 bytes
        vector<double> _e = {};                 // 8 bytes

        ClassDef(hit,
        1);
    };
}


#endif //SIMREX_HIT_H
