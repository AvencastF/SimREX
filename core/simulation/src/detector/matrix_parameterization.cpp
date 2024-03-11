//
// Created by Yulei on 2024/3/11.
//

#include "detector/matrix_parameterization.h"

namespace SimREX::Simulation {
    matrix_parameterization::matrix_parameterization(db::det_type type) : _type(type) {}

    void matrix_parameterization::ComputeTransformation(G4int copy_num, G4VPhysicalVolume* PV) const {

    }

    void matrix_parameterization::ComputeDimensions(
        G4Box& unit,
        G4int copy_num,
        const G4VPhysicalVolume* PV
    ) const {}
}
