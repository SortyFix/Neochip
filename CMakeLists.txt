cmake_minimum_required(VERSION 3.10)
project(neochip)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
    src/main.cpp
    src/display/display.cpp
    src/emu/core.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

include_directories(${SDL3_INCLUDE_DIRS})
add_executable(neochip ${SOURCES})

target_link_libraries(neochip ${SDL3_LIBRARIES})
