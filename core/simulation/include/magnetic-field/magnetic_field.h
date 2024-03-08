//
// Created by Yulei on 2024/3/9.
//

#ifndef SIMREX_MAGNETIC_FIELD_H
#define SIMREX_MAGNETIC_FIELD_H

// core dependencies
#include "global-event-model/logger.h"

// Geant4 dependencies
#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"

namespace SimREX::Simulation {

    class magnetic_field : public G4MagneticField {
    public:
        magnetic_field();

        ~magnetic_field() override;

        void GetFieldValue(const G4double point[4],double* bField ) const override;

    private:
        std::shared_ptr<spdlog::logger> _logger;
    };

}

#endif //SIMREX_MAGNETIC_FIELD_H
