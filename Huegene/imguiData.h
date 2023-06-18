#ifndef IMGUIDATA_H
#define IMGUIDATA_H

#include <vector>

enum class InitialValueOptions{
    Zero = 0,
    One = 1,
    Random = 2
};

struct ImguiData{
    bool chooseStartColour = false;
    float startColour[3];
    bool running = false;

    bool modifyHue = true;
    bool modifySaturation = true;
    bool modifyValue = true;

    int hueInitialOption = 2;
    std::vector<std::string> hueOptionsString = {"Zero", "360", "Random"};
    int saturationInitialOption = 1;
    std::vector<std::string> saturationOptionsString = {"Zero", "One", "Random"};
    int valueInitialOption = 1;
    std::vector<std::string> valueOptionsString = {"Zero", "One", "Random"};

    bool randomHerbivores = false;
    bool randomPlants = false;

};


#endif