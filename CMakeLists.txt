cmake_minimum_required(VERSION 3.10)
project(neochip)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(SOURCES 
    src/main.cpp
    src/display/display.cpp
    src/emu/core.cpp
)

find_package(PkgConfig REQUIRED)
find_package(Curses)
pkg_check_modules(SDL3 REQUIRED sdl3)
add_executable(neochip ${SOURCES})

target_include_directories(neochip PRIVATE
    ${SDL3_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(neochip PRIVATE ${SDL3_LIBRARIES} ${CURSES_LIBRARIES})
