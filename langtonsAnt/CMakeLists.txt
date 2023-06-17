cmake_minimum_required(VERSION 3.0.0)

find_package(SFML REQUIRED COMPONENTS graphics system window)

add_executable(LangtonsAnt main.cpp)

add_custom_command(
        TARGET LangtonsAnt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/LangtonsAnt/arial.ttf
                ${CMAKE_CURRENT_BINARY_DIR}/font/arial.ttf)

target_link_libraries(LangtonsAnt PRIVATE sfml-graphics sfml-system sfml-window)
