﻿cmake_minimum_required (VERSION 3.10)

project (AirlineTycoon)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2main REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include external libs.
add_subdirectory (libs)

# Include sub-projects.
add_subdirectory (src)

install(TARGETS AT DESTINATION "${PROJECT_INSTALL_DIR}")

