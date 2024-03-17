//
// Created by Yulei on 2024/3/7.
//

#ifndef SIMREX_GEMLINKDEF_H
#define SIMREX_GEMLINKDEF_H

#include <memory>
#include <vector>
#include <map>
#include <string>

#include "global-event-model/hit.h"
#include "global-event-model/particle.h"
#include "global-event-model/event.h"

using namespace SimREX::GEM;

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;


#pragma link C++ class hit+;
#pragma link C++ class std::vector<hit*>+;
#pragma link C++ class std::map<TString, std::vector<hit*>>+;

#pragma link C++ class particle+;
#pragma link C++ class std::vector<particle*>+;
#pragma link C++ class particle_state+;
#pragma link C++ class std::vector<particle_state*>+;

#pragma link C++ class SimREX::GEM::event+;
#pragma link C++ class std::vector<SimREX::GEM::event*>+;

#endif


#endif //SIMREX_GEMLINKDEF_H
