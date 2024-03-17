# Copyright(c) 2024 SimREX authors Distributed under the MIT License (http://opensource.org/licenses/MIT)
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Setup the project
project(SimREX VERSION 0.1.0)
add_definitions("-DSIMREX_VERSION=\"${PROJECT_VERSION}\"")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(OpenGL_GL_PREFERENCE GLVND)

# define path to macros and settings
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#################
# Build Options #
#################
# Add an option for building tests
option(BUILD_TESTS "Build tests" OFF)
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

###########################
# Setup Common Packages   #
###########################

# spdlog
if (NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
    link_libraries(spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif ()

# yaml-cpp
find_package(yaml-cpp REQUIRED)
link_libraries(yaml-cpp::yaml-cpp)

# XercesC
find_package(XercesC REQUIRED)
if (XercesC_FOUND)
    include_directories(${XercesC_INCLUDE_DIRS})
    link_libraries(${XercesC_LIBRARIES})
endif ()

# json
find_package(nlohmann_json 3.2.0 REQUIRED)

# OpenGL
find_package(OpenGL)

# CERN ROOT with version >= 6.30
find_package(ROOT 6.30 COMPONENTS Geom TMVA Eve Gui RGL EG MathCore ROOTDataFrame MultiProc REQUIRED)
IF (ROOT_FOUND)
    include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})
    include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
    link_libraries(${ROOT_LIBRARIES})
    link_directories(${ROOT_LIBRARY_DIR})
ENDIF ()

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


###################################
# Include Common External Tools   #
###################################

# morrisfranken/argparse: https://github.com/morrisfranken/argparse
include_directories(external/argparse/include)

# magic_enum: https://github.com/Neargye/magic_enum
include_directories(external/magic_enum/include)

###########
# Cores   #
###########
set(CMAKE_INSTALL_RPATH "@loader_path/../lib")

add_subdirectory(core)

#################
# Main Entrance #
#################
include_directories(${PROJECT_SOURCE_DIR}/core/global-event-model/include)
include_directories(${PROJECT_SOURCE_DIR}/core/simulation/include)
include_directories(${PROJECT_SOURCE_DIR}/core/reconstruction-analysis/include)
add_executable(SimREX SimREX.cpp)
target_link_libraries(SimREX PUBLIC GEM Simulation RecoAndAna)

install(TARGETS SimREX DESTINATION bin)

#########################
# Doxygen Documentation #
#########################
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Specify the input and output Doxyfile
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_PROJECT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/core)

    # Replace variables inside your Doxyfile.in
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else ()
    message(STATUS "Doxygen not found, unable to generate documentation")
endif ()
