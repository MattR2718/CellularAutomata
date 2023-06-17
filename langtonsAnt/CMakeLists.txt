cmake_minimum_required(VERSION 3.0.0)

find_package(SFML REQUIRED COMPONENTS graphics system window)

add_executable(LangtonsAnt main.cpp)

target_link_libraries(LangtonsAnt PRIVATE sfml-graphics sfml-system sfml-window)
