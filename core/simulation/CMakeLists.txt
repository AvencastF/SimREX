#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if (WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else ()
    find_package(Geant4 REQUIRED)
endif ()

include(${Geant4_USE_FILE})


# include global-event-model
include_directories(${PROJECT_SOURCE_DIR}/core/global-event-model/include)

# include simulation headers
include_directories(${PROJECT_SOURCE_DIR}/core/simulation/include)

file(GLOB_RECURSE sim_source "src/**/*.cpp")
file(GLOB_RECURSE sim_header "include/**/*.h")

add_library(Simulation SHARED ${sim_source} ${sim_header} src/simulation.cpp include/simulation.h)
add_library(SimREX::Simulation ALIAS Simulation)
target_link_libraries(Simulation PRIVATE ${Geant4_LIBRARIES} SimREX::GEM)

install(TARGETS Simulation LIBRARY DESTINATION lib)
