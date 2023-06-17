#ifndef CELL_H
#define CELL_H
enum class CellType : std::int8_t{
    None = 0,
    Herbivore = 1,
    Plant = 2
};

class Cell{
    public:
        CellType type = CellType::None;
        Colour colour;
        sf::RectangleShape shape;
        Cell(int x, int y, int s, const ImguiData& imguiData){
            this->type = CellType::None;
            
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(0, 360);
            std::uniform_real_distribution<> dist01(0.0, 1.0);

            float hue;
            float saturation;
            float value;

            switch(imguiData.hueInitialOption){
                case 0: hue = 0.0; break;
                case 1: hue = 360; break;
                case 2: hue = static_cast<float>(dist(rng)); break;
                default: hue = 0.0; break;
            }

            switch(imguiData.saturationInitialOption){
                case 0: saturation = 0.0; break;
                case 1: saturation = 1.0; break;
                case 2: saturation = static_cast<float>(dist01(rng)); break;
                default: saturation = 0.0; break;
            }

            switch(imguiData.valueInitialOption){
                case 0: value = 0.0; break;
                case 1: value = 1.0; break;
                case 2: value = static_cast<float>(dist01(rng)); break;
                default: value = 0.0; break;
            }

            this->colour = Colour(hue, saturation, value);

            this->shape.setSize(sf::Vector2f(static_cast<float>(s), static_cast<float>(s)));
            this->shape.setPosition(sf::Vector2f(static_cast<float>(x * s), static_cast<float>(y * s)));
        }
        void setType(CellType newtype){
            if(this->type == CellType::None){
                this->type = newtype;
            }
        }
        sf::RectangleShape draw(){
            this->shape.setFillColor(((type == CellType::None) ? sf::Color::Black : this->colour.getColour()));
            //this->shape.setFillColor(this->colour.getColour());

            return this->shape;
        }

};
#endif