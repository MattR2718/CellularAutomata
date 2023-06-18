#include <iostream>
#include <random>
#include <chrono>

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

void reInitialiseGrid(std::vector<Cell>& grid, int numHorizontalCells, int numVerticalCells, int cellSize, ImguiData& imguiData){
    for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
        int x = i % numHorizontalCells;
        int y = i / numHorizontalCells;
        grid[i] = Cell(x, y, cellSize, imguiData);
    }
}

void mutateplant(int index, std::vector<Cell>& grid, ImguiData& imguiData, std::mt19937& rng, 
            std::uniform_int_distribution<std::mt19937::result_type>& dist01,
            std::uniform_real_distribution<>& realdist05){
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

void mutateherb(int index, std::vector<Cell>& grid, ImguiData& imguiData, std::mt19937& rng, 
            std::uniform_int_distribution<std::mt19937::result_type>& dist01,
            std::uniform_real_distribution<>& realdist010){
    if(imguiData.modifyHue){
        grid[index].colour.h += (dist01(rng) - 0.5);
        if(grid[index].colour.h < 0){ grid[index].colour.h = 360; }
        if(grid[index].colour.h > 360){ grid[index].colour.h = 0; }
    }
    
    if(imguiData.modifySaturation){
        grid[index].colour.s += (realdist010(rng)/100 - 0.05);
        if(grid[index].colour.s < 0.0){ grid[index].colour.s = 0.0; }
        if(grid[index].colour.s > 1.0){ grid[index].colour.s = 1.0; }
    }

    if(imguiData.modifyValue){
        grid[index].colour.v += (realdist010(rng)/100 - 0.05);
        if(grid[index].colour.v < 0.0){ grid[index].colour.v = 0.0; }
        if(grid[index].colour.v > 1.0){ grid[index].colour.v = 1.0; }
    }
}

int main(){
    const int windowX = sf::VideoMode::getDesktopMode().width;
    const int windowY = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Huegene");
    //window.setFramerateLimit(5);
    //window.setFramerateLimit(30);
    //window.setFramerateLimit(60);
    auto _ = ImGui::SFML::Init(window);

    ImguiData imguiData;

    int cellSize = 25;
    int numHorizontalCells = windowX/cellSize;
    int numVerticalCells = windowY/cellSize;

    //std::cout<<numHorizontalCells<<' '<<numVerticalCells<<'\n';

    std::vector<Cell> grid;
    std::vector<Cell> tempGrid;
    for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
        int x = i % numHorizontalCells;
        int y = i / numHorizontalCells;
        grid.push_back(Cell(x, y, cellSize, imguiData));
    }
    tempGrid = grid;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist010(0, 10);
    std::uniform_int_distribution<std::mt19937::result_type> dist01(0, 1);
    std::uniform_int_distribution<std::mt19937::result_type> dist02(0, 2);
    std::uniform_int_distribution<std::mt19937::result_type> dist0max(0, grid.size());

    std::uniform_real_distribution<> realdist05(0.0, 5.0);
    std::uniform_real_distribution<> realdist010(0.0, 10.0);
    //Cell grid[100000];

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
    float fps;

    sf::Clock deltaClock;
    while(window.isOpen()){
        start = std::chrono::high_resolution_clock::now();

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
                    tempGrid[(my / cellSize) * numHorizontalCells + (mx / cellSize)].type = CellType::Plant;
                    if(imguiData.chooseStartColour){ tempGrid[(my / cellSize) * numHorizontalCells + (mx / cellSize)].colour = Colour(Colour::rgb2hsv(imguiData.startColour)); }
                    else{ tempGrid[(my / cellSize) * numHorizontalCells + (mx / cellSize)].colour = Cell(0, 0, 1, imguiData).colour; }
                }else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    auto[mx, my]{mousePos};
                    tempGrid[(my / cellSize) * numHorizontalCells + (mx / cellSize)].type = CellType::Herbivore;
                    if(imguiData.chooseStartColour){ tempGrid[(my / cellSize) * numHorizontalCells + (mx / cellSize)].colour = Colour(Colour::rgb2hsv(imguiData.startColour)); }
                }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            reInitialiseGrid(grid, numHorizontalCells, numVerticalCells, cellSize, imguiData);
            tempGrid = grid;
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color::Black);

        if(imguiData.randomHerbivores && !dist01(rng)){
            grid[dist0max(rng)%grid.size()].type = CellType::Herbivore;
        }
        if(imguiData.randomPlants && !dist01(rng)){
            grid[dist0max(rng)%grid.size()].type = CellType::Plant;
        }

        //Update Cells
        if(imguiData.running){
            for(int i = 0; i < (numHorizontalCells * numVerticalCells); i++){
                //Expand Plants
                if(grid[i].type == CellType::Plant){
                    int x = i % numHorizontalCells;
                    int y = i / numHorizontalCells;
                    for(int j = -1; j < 2; j++){
                        for(int k = -1; k < 2; k++){
                            if((y + k) < 0 || (y + k) > (numVerticalCells - 1) || (x + j) < 0 || (x + j) > (numHorizontalCells - 1)){ continue; }
                            int index = (y + k) * numHorizontalCells + (x + j);
                            if(grid[index].type == CellType::None && !dist010(rng)){
                                tempGrid[index].type = CellType::Plant;
                                tempGrid[index].colour = grid[y * numHorizontalCells + x].colour;
                                
                                mutateplant(index, tempGrid, imguiData, rng, dist01, realdist05);

                                /* if(imguiData.modifyHue){
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
                                } */
                                
                            }
                        }
                    }
                //Expand Herbivores
                }else if(grid[i].type == CellType::Herbivore){
                    int x = i % numHorizontalCells;
                    int y = i / numHorizontalCells;
                    std::vector<int> surroundingPlants;
                    for(int j = -1; j < 2; j++){
                        for(int k = -1; k < 2; k++){
                            if((y + k) < 0 || (y + k) > (numVerticalCells - 1) || (x + j) < 0 || (x + j) > (numHorizontalCells - 1)){ continue; }
                            int index = (y + k) * numHorizontalCells + (x + j);
                            if(grid[index].type == CellType::Plant){
                                surroundingPlants.push_back(index);
                            }
                        }
                    }
                    if(surroundingPlants.size() == 0){
                        int nextx = x + dist02(rng) - 1;
                        int nexty = y + dist02(rng) - 1;
                        if(nextx < 0 || nextx > (numHorizontalCells - 1) || nexty < 0 || nexty > (numVerticalCells - 1)){ break; }
                        //tempGrid[nexty * numHorizontalCells + nextx] = grid[y * numHorizontalCells + x];
                        tempGrid[nexty * numHorizontalCells + nextx].colour = grid[y * numHorizontalCells + x].colour;
                        tempGrid[nexty * numHorizontalCells + nextx].type = CellType::Herbivore;
                        tempGrid[y * numHorizontalCells + x].type = CellType::None;
                        //tempGrid[y * numHorizontalCells + x] = Cell(x, y, cellSize, imguiData);
                    }else{
                        for(auto ind : surroundingPlants){
                            float diff = std::max(grid[i].colour.h, grid[ind].colour.h) - std::min(grid[i].colour.h, grid[ind].colour.h);
                            if(diff > 180){ diff = 360 - diff; }
                            diff *= 2;

                            if(diff < 20){
                                if(dist010(rng) < 8){
                                    //std::cout<<diff<<'\n';
                                    tempGrid[ind].type = CellType::Herbivore;
                                    
                                    mutateherb(ind, tempGrid, imguiData, rng, dist01, realdist010);
                                    
                                }
                            }
                        }
                        tempGrid[i].type = CellType::None;
                    }
                }
            }
        }



        grid = tempGrid;

        //Draw all cells
        for(int i = 0; i < (numHorizontalCells * numVerticalCells); i++){
            int x = i % numHorizontalCells;
            int y = i / numHorizontalCells;
            grid[i].draw(&window);
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
                tempGrid = grid;
            }

            if(ImGui::Button("GreyScale - White")){
                imguiData.saturationInitialOption = 0;
                imguiData.valueInitialOption = 1;
                imguiData.modifyValue = true;
                imguiData.modifyHue = false;
                imguiData.modifySaturation = false;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
                tempGrid = grid;
            }

            if(ImGui::Button("Dark Colours")){
                imguiData.saturationInitialOption = 0;
                imguiData.valueInitialOption = 0;
                imguiData.hueInitialOption = 2;
                imguiData.modifyValue = true;
                imguiData.modifyHue = true;
                imguiData.modifySaturation = true;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
                tempGrid = grid;
            }

            if(ImGui::Button("Light Colours")){
                imguiData.saturationInitialOption = 0;
                imguiData.valueInitialOption = 1;
                imguiData.hueInitialOption = 2;
                imguiData.modifyValue = true;
                imguiData.modifyHue = true;
                imguiData.modifySaturation = true;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
                tempGrid = grid;
            }

            if(ImGui::Button("Modify All")){
                imguiData.saturationInitialOption = 1;
                imguiData.valueInitialOption = 1;
                imguiData.hueInitialOption = 2;
                imguiData.modifyValue = true;
                imguiData.modifyHue = true;
                imguiData.modifySaturation = true;
                resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
                tempGrid = grid;
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
            tempGrid = grid;
        }

        ImGui::Checkbox("Random Plants", &imguiData.randomPlants);
        ImGui::Checkbox("Random Herbivores", &imguiData.randomHerbivores);

        if(ImGui::Button("Clear")){
            reInitialiseGrid(grid, numHorizontalCells, numVerticalCells, cellSize, imguiData);
            tempGrid = grid;
        }

        if(ImGui::SliderInt("Cell Size", &cellSize, 5, 100)){
            setCellSize(cellSize, numHorizontalCells, numVerticalCells, windowX, windowY);
            grid.clear();
            for(int i = 0; i < numHorizontalCells * numVerticalCells; i++){
                int x = i % numHorizontalCells;
                int y = i / numHorizontalCells;
                grid.push_back(Cell(x, y, cellSize, imguiData));
            }            
            tempGrid = grid;
        }

        if(ImGui::Button("Reset All None Value Cells")){
            resetAllNoneCells(grid, cellSize, imguiData, numHorizontalCells, numVerticalCells);
            tempGrid = grid;
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

        ImGui::Text(std::to_string(fps).c_str());

        ImGui::End();

        end = std::chrono::high_resolution_clock::now();
        fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        //std::cout<<fps<<'\n';

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}