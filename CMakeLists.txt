# Copyright(c) 2024 SimREX authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

# Setup the project
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(SimREX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(OpenGL_GL_PREFERENCE GLVND)

# define path to macros and settings
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

###########################
# Setup Common Packages   #
###########################

# spdlog
if(NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
#    target_link_libraries(example PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()

# yaml-cpp
find_package(yaml-cpp REQUIRED)
link_libraries(yaml-cpp)

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
find_package(ROOT 6.30 COMPONENTS Geom TMVA Eve Gui RGL EG REQUIRED )
IF (ROOT_FOUND)
    include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})
    include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
    link_libraries(${ROOT_LIBRARIES})
    link_directories(${ROOT_LIBRARY_DIR})
ENDIF ()

###################################
# Include Common External Tools   #
###################################

# morrisfranken/argparse
include_directories(external/argparse/include)



###########
# Cores   #
###########
add_subdirectory(core)