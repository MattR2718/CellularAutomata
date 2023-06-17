cmake_minimum_required(VERSION 3.0.0)

find_package(SFML REQUIRED COMPONENTS graphics system window)

add_executable(GameOfLife main.cpp)

target_link_libraries(GameOfLife PRIVATE sfml-graphics sfml-system sfml-window)