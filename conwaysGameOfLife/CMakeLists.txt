cmake_minimum_required(VERSION 3.0.0)

find_package(SFML REQUIRED COMPONENTS graphics system window)

add_executable(GameOfLife main.cpp)

add_custom_command(
        TARGET GameOfLife POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/ConwaysGameOfLife/arial.ttf
                ${CMAKE_CURRENT_BINARY_DIR}/font/arial.ttf)

target_link_libraries(GameOfLife PRIVATE sfml-graphics sfml-system sfml-window)