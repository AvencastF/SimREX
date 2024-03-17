# include global-event-model
include_directories(${PROJECT_SOURCE_DIR}/core/global-event-model/include)

# include simulation headers
include_directories(${PROJECT_SOURCE_DIR}/core/simulation/include)

file(GLOB_RECURSE sim_source "src/**/*.cpp")
file(GLOB_RECURSE sim_header "include/**/*.h")

add_library(Simulation SHARED ${sim_source} ${sim_header} src/simulation.cpp include/simulation.h)
target_link_libraries(Simulation PRIVATE ${Geant4_LIBRARIES} GEM)

install(TARGETS Simulation LIBRARY DESTINATION lib)
