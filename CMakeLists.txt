cmake_minimum_required(VERSION 3.10)
project(ParticleRender CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Header-only library
add_library(mylib INTERFACE)

# Propagate include path to dependents
target_include_directories(mylib
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(particle_render main.cpp)

target_link_libraries(particle_render
        sfml-graphics
        sfml-window
        sfml-system
        mylib
)
