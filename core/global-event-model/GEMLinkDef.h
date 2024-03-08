//
// Created by Yulei on 2024/3/7.
//

#ifndef SIMREX_GEMLINKDEF_H
#define SIMREX_GEMLINKDEF_H

#include <memory>
#include <vector>
#include <map>
#include <string>

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;


#pragma link C++ class SimREX::GEM::hit+;
#pragma link C++ class std::vector<SimREX::GEM::hit*>+;

#pragma link C++ class SimREX::GEM::particle+;
#pragma link C++ class std::vector<SimREX::GEM::particle*>+;
#pragma link C++ class SimREX::GEM::particle_state+;
#pragma link C++ class std::vector<SimREX::GEM::particle_state*>+;

#pragma link C++ class SimREX::GEM::event+;
#pragma link C++ class std::vector<SimREX::GEM::event*>+;

#endif


#endif //SIMREX_GEMLINKDEF_H
