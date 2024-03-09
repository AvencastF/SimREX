//
// Created by Yulei on 2024/3/9.
//

#include "magnetic-field/magnetic_field.h"

#include "G4Threading.hh"

SimREX::Simulation::magnetic_field::magnetic_field() {
    _logger = SimREX::GEM::LoggerManager::getInstance()->createLogger(
            std::format("Magnetic Field: {}", G4Threading::G4GetThreadId()));
//    _logger->info("constructor called.");
}

SimREX::Simulation::magnetic_field::~magnetic_field() {
//    _logger->info("destructor called.");
}

void SimREX::Simulation::magnetic_field::GetFieldValue(const G4double *point, double *bField) const {
    bField[0] = 0.;
    bField[1] = 0.;
    bField[2] = 0.;
}
