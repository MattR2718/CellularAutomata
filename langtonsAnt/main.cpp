#include <SFML/Graphics.hpp>
#include <iostream>

struct Ant {
    int x;
    int y;
    int direction;
};

int main()
{
    const int lineWidth = 1;
    const int cellSize = 25;
    const int windowX = sf::VideoMode::getDesktopMode().width;
    const int windowY = sf::VideoMode::getDesktopMode().height;
    int step = 0;
    bool running = 0;
    bool board[500][500];
    Ant antData;
    antData.x = (windowX / cellSize)/2;
    antData.y = (windowY / cellSize)/2;
    antData.direction = 0;

    //Clear board
    for (int x = 0; x < windowX / cellSize; x++) {
        for (int y = 0; y < windowY / cellSize; y++) {
            board[x][y] = 1;
        }
    }

    //Font
    sf::Font font;
    font.loadFromFile("../font/arial.ttf");

    // create the window
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Langton's Ant");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        //
        //Grid
        //Horizontal
        sf::RectangleShape hLine(sf::Vector2f(static_cast<float>(windowX), lineWidth));
        hLine.setFillColor(sf::Color(100, 100, 100));
        for (int y = 0; y < windowY; y += cellSize) {
            hLine.setPosition(0, static_cast<float>(y));
            window.draw(hLine);
        }
        //Vertical
        sf::RectangleShape vLine(sf::Vector2f(lineWidth, static_cast<float>(windowY)));
        vLine.setFillColor(sf::Color(100, 100, 100));
        for (int x = 0; x < windowX; x += cellSize) {
            vLine.setPosition(static_cast<float>(x), 0);
            window.draw(vLine);
        }

        //Draw board
        sf::RectangleShape cell(sf::Vector2f(cellSize-lineWidth, cellSize-lineWidth));
        for (int x = 0; x < windowX / cellSize; x++) {
            for (int y = 0; y < windowY / cellSize; y++) {
                if (board[x][y]) { cell.setFillColor(sf::Color(255, 255, 255));}
                else { cell.setFillColor(sf::Color(0, 0, 0)); }
                cell.setPosition(static_cast<float>((x * cellSize)+lineWidth), static_cast<float>((y * cellSize)+lineWidth));
                window.draw(cell);
            }
        }
        //Draw ant
        sf::RectangleShape ant(sf::Vector2f(cellSize - lineWidth, cellSize - lineWidth));
        ant.setPosition(static_cast<float>((antData.x * cellSize) + lineWidth), static_cast<float>((antData.y * cellSize) + lineWidth));
        ant.setFillColor(sf::Color(255, 0, 0));
        window.draw(ant);

        //Draw step count
        sf::Text text("Steps: " + std::to_string(step), font);
        text.setCharacterSize(50);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Red);
        window.draw(text);

        if (!running) {
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    //std::cout << "Running" << '\n';
                    running = 1;
                }
            }
        }
        if (running) {
            //Pause
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    //std::cout << "Stopped" << '\n';
                    running = 0;
                }
            }

            //Game Playing

            //At a white square, turn 90° clockwise, flip the color of the square, move forward one unit
            //At a black square, turn 90° counter - clockwise, flip the color of the square, move forward one unit
            if (board[antData.x][antData.y]) {
                antData.direction = (antData.direction + 1) % 4;
                board[antData.x][antData.y] = 0;
            }
            else if (!board[antData.x][antData.y]) {
                antData.direction = (antData.direction + 3) % 4;
                board[antData.x][antData.y] = 1;
            }
            switch (antData.direction) {
            case (0):
                antData.y--;
                break;
            case (1):
                antData.x++;
                break;
            case (2):
                antData.y++;
                break;
            case (3):
                antData.x--;
                break;
            }
            step++;

        }

        // end the current frame
        window.display();
    }
}
