include_directories(include)

# include global-event-model
include_directories(${PROJECT_SOURCE_DIR}/global-event-model/include)

# include simulation headers
include_directories(${PROJECT_SOURCE_DIR}/simulation/include)

file(GLOB_RECURSE sim_source "src/**/*.cpp")
file(GLOB_RECURSE sim_header "include/**/*.h")

add_library(Simulation SHARED ${sim_source} ${sim_header} src/simulation.cpp include/simulation.h)
add_library(SimREX::Simulation ALIAS Simulation)
target_link_libraries(Simulation PRIVATE ${Geant4_LIBRARIES} SimREX::GEM)

install(TARGETS Simulation LIBRARY DESTINATION lib)
