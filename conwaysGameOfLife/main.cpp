#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <utility>
#include <vector>

int main()
{
    int wSize = 1000;

    const int WIDTH = sf::VideoMode::getDesktopMode().width;
    const int HEIGHT = sf::VideoMode::getDesktopMode().height;

    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");


    const float lineWidth = 1;
    int cellSize = 20;
    const int horizontalCellsNum = WIDTH/cellSize;
    const int verticalCellsNum = HEIGHT / cellSize;
    bool running = 0;

    bool board[500][500];
    bool tempBoard[500][500];
    for (int x = 0; x < horizontalCellsNum + 2; x++) {
        for (int y = 0; y < verticalCellsNum + 2; y++) {
            board[x][y] = 0;
            tempBoard[x][y] = 0;
        }
    }

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
        //
        //
        //
        //Set up game board and cells
        //Background
        sf::RectangleShape background(sf::Vector2f(static_cast<float>(WIDTH), static_cast<float>(HEIGHT)));
        background.setFillColor(sf::Color(25, 25, 25));
        window.draw(background);

        //Grid
        sf::RectangleShape hline(sf::Vector2f(static_cast<float>(WIDTH), lineWidth));
        hline.setFillColor(sf::Color(100, 100, 100));

        //horizontal
        for (int y = 0; y < HEIGHT - lineWidth; y += cellSize) {
            hline.setPosition(0, static_cast<float>(y));
            window.draw(hline);
        }
        hline.setPosition(0, HEIGHT - lineWidth);
        window.draw(hline);

        //vertical
        sf::RectangleShape vline(sf::Vector2f(lineWidth, static_cast<float>(HEIGHT)));
        vline.setFillColor(sf::Color(100, 100, 100));
        for (int x = 0; x < WIDTH - lineWidth; x += cellSize) {
            vline.setPosition(static_cast<float>(x), 0);
            window.draw(vline);
        }
        vline.setPosition(WIDTH - lineWidth, 0);
        window.draw(vline);

        //Instructions
        sf::Font font;
        font.loadFromFile("../font/arial.ttf");
        sf::Text text("SPACE to run  ESC to pause and edit  LCLICK to add cells  RCLICK to remove cells", font);
        text.setCharacterSize(50);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::White);
        text.setPosition(0, 0);
        window.draw(text);

        sf::RectangleShape cell(sf::Vector2f(cellSize - lineWidth, cellSize - lineWidth));
        //Adding cells
        if (!running) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int x = static_cast<int>(std::floor(sf::Mouse::getPosition(window).x / cellSize) * cellSize);
                int y = static_cast<int>(std::floor(sf::Mouse::getPosition(window).y / cellSize) * cellSize);
                board[(x / cellSize) + 1][(y / cellSize) + 1] = 1;
            }
        }

        //Remove cells
        if (!running) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                int x = static_cast<int>(floor(sf::Mouse::getPosition(window).x / cellSize) * cellSize);
                int y = static_cast<int>(floor(sf::Mouse::getPosition(window).y / cellSize) * cellSize);
                //board[(x / cellSize) + 1][(y / cellSize) + 1] = std::make_tuple(x/cellSize, y/cellSize, 0);
                board[(x / cellSize) + 1][(y / cellSize) + 1] = 0;
            }
        }

        //Draw cells
        cell.setFillColor(sf::Color(0, 0, 255));
        for (int x = 1; x < horizontalCellsNum+1; x++) {
            for (int y = 1; y < verticalCellsNum+1; y++) {
                cell.setPosition(((x - 1) * cellSize) + lineWidth, ((y - 1) * cellSize) + lineWidth);
                if (board[x][y]) {
                    window.draw(cell);
                }
            }
        }


        //Run Game
        if (!running) {
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    //std::cout << "Running" << '\n';
                    running = 1;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
                for (int x = 0; x < horizontalCellsNum + 2; x++) {
                    for (int y = 0; y < verticalCellsNum + 2; y++) {
                        board[x][y] = 0;
                        tempBoard[x][y] = 0;
                    }
                }
            }
        }
        if (running) {
            //Pause
            //std::cout << "RUNNING" << '\n';
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    running = 0;
                }
            }

            //Game Playing
            //bool tempBoard[size + 2][size + 2];

            for (int c = 0; c < horizontalCellsNum + 2; c++) {
                for (int v = 0; v < verticalCellsNum + 2; v++) {
                    tempBoard[c][v] = board[c][v];
                }
            }

            /* for(auto& c : tempBoard){
                for(auto& d : c){
                    d = 0;
                }
            } */

            /* for (int x = 0; x < horizontalCellsNum + 2; x++) {
                tempBoard[x][0] = 0;
                tempBoard[0][x] = 0;
                tempBoard[x][verticalCellsNum + 1] = 0;
                tempBoard[horizontalCellsNum + 1][x] = 0;
            } */

            //if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                for (int k = 1; k < horizontalCellsNum + 1; k++) {
                    for (int l = 1; l < verticalCellsNum + 1; l++) {

                        //Rule 1
                        //Any live cell with fewer than two live neighbours dies, as if by underpopulation.
                        //Rule 2
                        //Any live cell with two or three live neighbours lives on to the next generation.
                        //Rule 3
                        //Any live cell with more than three live neighbours dies, as if by overpopulation.
                        //Rule 4
                        //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

                        int i, j;
                        int count = 0;
                        for (i = k - 1; i <= k + 1; i++) {
                            for (j = l - 1; j <= l + 1; j++) {
                                count += board[i][j];//increase the count is neighbor is alive
                            }
                        }
                        count -= board[k][l];//reduce count, since cell is not its own neighbor

                        //Rule 1
                        if ((tempBoard[k][l]) && (count < 2)) {
                            tempBoard[k][l] = 0;
                        }
                        //Rule 3
                        else if ((tempBoard[k][l]) && (count > 3)) {
                            tempBoard[k][l] = 0;
                        }
                        else if ((!tempBoard[k][l]) && (count == 3)) { tempBoard[k][l] = 1; }
                    }
                }

                //Switch boards back
                for (int c = 0; c < horizontalCellsNum + 2; c++) {
                    for (int v = 0; v < verticalCellsNum + 2; v++) {
                        board[c][v] = tempBoard[c][v];
                    }
                }
           // }
             
        }

        // end the current frame
        window.display();
    }
}
