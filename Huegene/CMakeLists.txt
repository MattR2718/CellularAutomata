cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML REQUIRED COMPONENTS graphics system window)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

find_package(unofficial-nativefiledialog CONFIG REQUIRED)

add_executable(Huegene main.cpp)

add_custom_command(
        TARGET Huegene POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/LangtonsAnt/arial.ttf
                ${CMAKE_CURRENT_BINARY_DIR}/font/arial.ttf)

target_link_libraries(Huegene PRIVATE 
                        imgui::imgui
                        sfml-graphics sfml-system sfml-window
                        ImGui-SFML::ImGui-SFML
                        unofficial::nativefiledialog::nfd
)
