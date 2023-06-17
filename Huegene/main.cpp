#include <iostream>
#include <random>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <nfd.h>

#include "Colour.h"
#include "imguiData.h"
#include "Cell.h"

void resetAllNoneCells(std::vector<Cell>& grid, const int cellSize, const ImguiData& imguiData, int numHorizontalCells, int numVerticalCells){
    for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
        int x = i % numHorizontalCells;
        int y = i / numHorizontalCells;
        if(grid[i].type == CellType::None){
            grid[i] = Cell(x, y, cellSize, imguiData);
        }
    }
}

void setCellSize(int cellSize, int& numHV, int& numVV, const int WX, const int WY){
    numHV = WX/cellSize;
    numVV = WY/cellSize;
}

int main(){
    const int windowX = sf::VideoMode::getDesktopMode().width;
    const int windowY = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Huegene");
    auto _ = ImGui::SFML::Init(window);

    ImguiData imguiData;

    int cellSize = 25;
    int numHorizontalCells = windowX/cellSize;
    int numVerticalCells = windowY/cellSize;

    //std::cout<<numHorizontalCells<<' '<<numVerticalCells<<'\n';

    std::vector<Cell> grid;
    for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
        int x = i % numHorizontalCells;
        int y = i / numHorizontalCells;
        grid.push_back(Cell(x, y, cellSize, imguiData));
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist010(0, 10);
    std::uniform_int_distribution<std::mt19937::result_type> dist01(0, 1);
    std::uniform_real_distribution<> realdist05(0.0, 5.0);
    //Cell grid[100000];

    sf::Clock deltaClock;
    while(window.isOpen()){
        sf::Event event;
        ImGui::SFML::ProcessEvent(window, event);
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed && !ImGui::GetIO().WantCaptureMouse){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    auto[mx, my]{mousePos};
                    grid[(my / cellSize) * numHorizontalCells + (mx / cellSize)].type = CellType::Plant;
                    if(imguiData.chooseStartColour){ grid[(my / cellSize) * numHorizontalCells + (mx / cellSize)].colour = Colour(Colour::rgb2hsv(imguiData.startColour)); }
                }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
                int x = i % numHorizontalCells;
                int y = i / numHorizontalCells;
                grid[i] = Cell(x, y, cellSize, imguiData);
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color::Black);

        //Expand plants
        if(imguiData.running){
            for(int i = 0; i < (numHorizontalCells * numVerticalCells); i++){
                if(grid[i].type == CellType::Plant){
                    int x = i % numHorizontalCells;
                    int y = i / numHorizontalCells;
                    for(int j = -1; j < 2; j++){
                        for(int k = -1; k < 2; k++){
                            if((y + k) < 0 || (y + k) > (numVerticalCells - 1) || (x + j) < 0 || (x + j) > (numHorizontalCells - 1)){ continue; }
                            int index = (y + k) * numHorizontalCells + (x + j);
                            if(grid[index].type == CellType::None && !dist010(rng)){
                                grid[index].type = CellType::Plant;
                                grid[index].colour = grid[y * numHorizontalCells + x].colour;
                                
                                if(imguiData.modifyHue){
                                    grid[index].colour.h += (dist01(rng) - 0.5);
                                    if(grid[index].colour.h < 0){ grid[index].colour.h = 360; }
                                    if(grid[index].colour.h > 360){ grid[index].colour.h = 0; }
                                }
                                
                                if(imguiData.modifySaturation){
                                    grid[index].colour.s += (realdist05(rng)/100 - 0.025);
                                    if(grid[index].colour.s < 0.0){ grid[index].colour.s = 0.0; }
                                    if(grid[index].colour.s > 1.0){ grid[index].colour.s = 1.0; }
                                }

                                if(imguiData.modifyValue){
                                    grid[index].colour.v += (realdist05(rng)/100 - 0.025);
                                    if(grid[index].colour.v < 0.0){ grid[index].colour.v = 0.0; }
                                    if(grid[index].colour.v > 1.0){ grid[index].colour.v = 1.0; }
                                }
                                
                            }
                        }
                    }
                }
            }
        }

        //Draw all cells
        for(int i = 0; i < (numHorizontalCells * numVerticalCells); i++){
            int x = i % numHorizontalCells;
            int y = i / numHorizontalCells;
            window.draw(grid[i].draw());
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
        ImGui::Begin("OPTIONS", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowFontScale(2.5);
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::Checkbox("Choose Click Colour", &imguiData.chooseStartColour);
        if(imguiData.chooseStartColour){
            ImGui::ColorEdit3("Pick Colour", imguiData.startColour);
        }

        if(ImGui::CollapsingHeader("Presets")){
            if(ImGui::Button("GreyScale - Black")){
                imguiData.saturationInitialOption = 0;
                imguiData.valueInitialOption = 0;
                imguiData.modifyValue = true;
                imguiData.modifyHue = false;
                imguiData.modifySaturation = false;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
            }

            if(ImGui::Button("GreyScale - White")){
                imguiData.saturationInitialOption = 0;
                imguiData.valueInitialOption = 1;
                imguiData.modifyValue = true;
                imguiData.modifyHue = false;
                imguiData.modifySaturation = false;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
            }

            if(ImGui::Button("Dark Colours")){
                imguiData.saturationInitialOption = 0;
                imguiData.valueInitialOption = 0;
                imguiData.hueInitialOption = 2;
                imguiData.modifyValue = true;
                imguiData.modifyHue = true;
                imguiData.modifySaturation = true;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
            }

            if(ImGui::Button("Light Colours")){
                imguiData.saturationInitialOption = 0;
                imguiData.valueInitialOption = 1;
                imguiData.hueInitialOption = 2;
                imguiData.modifyValue = true;
                imguiData.modifyHue = true;
                imguiData.modifySaturation = true;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
            }

            if(ImGui::Button("Modify All")){
                imguiData.saturationInitialOption = 1;
                imguiData.valueInitialOption = 1;
                imguiData.hueInitialOption = 2;
                imguiData.modifyValue = true;
                imguiData.modifyHue = true;
                imguiData.modifySaturation = true;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
            }
        }

        ImGui::Checkbox("Modify Hue", &imguiData.modifyHue);
        ImGui::Checkbox("Modify Saturation", &imguiData.modifySaturation);
        ImGui::Checkbox("Modify Value", &imguiData.modifyValue);

        if(ImGui::BeginCombo("Initial Hue", imguiData.hueOptionsString[imguiData.hueInitialOption].c_str())){
            for(std::uint8_t i = 0; i < imguiData.hueOptionsString.size(); i++){
                const bool isSelected = (imguiData.hueInitialOption == i);
                if (ImGui::Selectable(imguiData.hueOptionsString[i].c_str(), isSelected)) {
                    imguiData.hueInitialOption = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if(ImGui::BeginCombo("Initial Saturation", imguiData.saturationOptionsString[imguiData.saturationInitialOption].c_str())){
            for(std::uint8_t i = 0; i < imguiData.saturationOptionsString.size(); i++){
                const bool isSelected = (imguiData.saturationInitialOption == i);
                if (ImGui::Selectable(imguiData.saturationOptionsString[i].c_str(), isSelected)) {
                    imguiData.saturationInitialOption = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if(ImGui::BeginCombo("Initial Value", imguiData.valueOptionsString[imguiData.valueInitialOption].c_str())){
            for(std::uint8_t i = 0; i < imguiData.valueOptionsString.size(); i++){
                const bool isSelected = (imguiData.valueInitialOption == i);
                if (ImGui::Selectable(imguiData.valueOptionsString[i].c_str(), isSelected)) {
                    imguiData.valueInitialOption = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if(ImGui::Checkbox("Running", &imguiData.running)){
            /* for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
                int x = i % numHorizontalCells;
                int y = i / numHorizontalCells;
                if(grid[i].type == CellType::None){
                    grid[i] = Cell(x, y, cellSize, imguiData);
                }
            } */
            resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
        }

        if(ImGui::SliderInt("Cell Size", &cellSize, 5, 100)){
            setCellSize(cellSize, numHorizontalCells, numVerticalCells, windowX, windowY);
            grid.clear();
            for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
                int x = i % numHorizontalCells;
                int y = i / numHorizontalCells;
                grid.push_back(Cell(x, y, cellSize, imguiData));
            }            
        }

        if(ImGui::Button("Reset All None Value Cells")){
            resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
        }

        if(ImGui::Button("Save Image")){
            //Variable to store path
            nfdchar_t *outPath = NULL;
            //Open up save dialog and store path chosen by user into outpath
            nfdresult_t result = NFD_SaveDialog("png;", NULL, &outPath);
            if(result == NFD_OKAY){
                sf::Vector2u windowSize = window.getSize();
                sf::Texture texture;
                texture.create(windowSize.x, windowSize.y);
                texture.update(window);
                sf::Image screenshot = texture.copyToImage();
                screenshot.saveToFile(std::string(outPath) + ".png");
            }else{
                // ERROR
            }
        }

        ImGui::End();

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}