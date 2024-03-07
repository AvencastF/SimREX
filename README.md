# SimREX

SimREX is the next-generation Simulation and Reconstruction Framework for the future fixed-target Experiment. 

## Software Requirements
- CMake 3.15 or later
- ROOT 6.30 or later
- Geant4 11.2 or later
- yaml-cpp
- spdlog
- [nlohmann/json](https://github.com/nlohmann/json)
- xerces-c (XML parser for GDML)
- [VecGeom](https://gitlab.cern.ch/VecGeom/VecGeom)  ([G4VecGeomNav](https://gitlab.cern.ch/VecGeom/g4vecgeomnav)) *Experimental*



## Software Installation

### CERN ROOT
```bash
cmake ../root-master -Dpython3=ON -DPYTHON_EXECUTABLE=/Users/avencast/miniconda3/bin/python3 -DCMAKE_INSTALL_PREFIX=/Applications/ROOT/install  -DCMAKE_CXX_STANDARD=20 -Dbuiltin_glew=ON
```

### Geant4.11
```bash
cmake ../src -DCMAKE_INSTALL_PREFIX=/Applications/Geant4.11/install -DGEANT4_USE_GDML=ON -DGEANT4_USE_QT=ON -DCMAKE_CXX_STANDARD=20 -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_SYSTEM_EXPAT=OFF 
```

### VecGeom
```bash
# VecGeom
cmake ../src.VecGeom -DCMAKE_INSTALL_PREFIX=/Applications/VecGeom/install -DVECGEOM_BUILTIN_VECCORE=ON -DVECGEOM_GDML=ON -DCMAKE_CXX_STANDARD=20

# G4VecGeomNav
cmake ../src.G4VecGeomNav -DCMAKE_INSTALL_PREFIX=/Applications/VecGeom/install -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH=/Applications/VecGeom/install/lib/cmake
```
