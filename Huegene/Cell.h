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
        sf::RectangleShape plantShape;
        sf::CircleShape herbivoreShape;
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

            this->plantShape.setSize(sf::Vector2f(static_cast<float>(s), static_cast<float>(s)));
            this->plantShape.setPosition(sf::Vector2f(static_cast<float>(x * s), static_cast<float>(y * s)));
            
            this->herbivoreShape.setPosition(sf::Vector2f(static_cast<float>(x * s), static_cast<float>(y * s)));
            this->herbivoreShape.setRadius(static_cast<float>(s/2));
        }
        void setType(CellType newtype){
            if(this->type == CellType::None){
                this->type = newtype;
            }
        }
        void draw(sf::RenderWindow* window){
            this->herbivoreShape.setFillColor(((type == CellType::None) ? sf::Color::Black : this->colour.getColour()));
            this->plantShape.setFillColor(((type == CellType::None) ? sf::Color::Black : this->colour.getColour()));
            //this->shape.setFillColor(this->colour.getColour());

            //window->draw(((this->type == CellType::Plant) ? this->plantShape : this->herbivoreShape));
            
            //if(this->type == CellType::Herbivore){ window->draw(this->herbivoreShape); }
            //else{ window->draw(this->plantShape); }
            if(this->type == CellType::Plant){ window->draw(this->plantShape); }
        }

};
#endif