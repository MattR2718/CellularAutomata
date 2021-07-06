#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <utility>

int main()
{
    int wSize = 1000;
    // create the window
    sf::RenderWindow window(sf::VideoMode(wSize, wSize), "My window");


    const float width = 1;
    const int size = 100;
    int cellSize = round(wSize / size);
    bool running = 0;

    bool board[size + 2][size + 2];
    bool tempBoard[size + 2][size + 2];
    for (int x = 0; x < size + 2; x++) {
        for (int y = 0; y < size + 2; y++) {
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
        sf::RectangleShape background(sf::Vector2f(wSize, wSize));
        background.setFillColor(sf::Color(0, 0, 0));
        window.draw(background);

        //Grid
        sf::RectangleShape hline(sf::Vector2f(wSize, width));
        hline.setFillColor(sf::Color(100, 100, 100));

        //horizontal
        for (int y = 0; y < wSize - width; y += wSize/size) {
            hline.setPosition(0, y);
            window.draw(hline);
        }
        hline.setPosition(0, wSize - width);
        window.draw(hline);

        //vertical
        sf::RectangleShape vline(sf::Vector2f(width, wSize));
        vline.setFillColor(sf::Color(100, 100, 100));
        for (int x = 0; x < wSize - width; x += wSize / size) {
            vline.setPosition(x, 0);
            window.draw(vline);
        }
        vline.setPosition(wSize - width, 0);
        window.draw(vline);

        //Instructions
        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text text("SPACE to run  ESC to pause and edit  LCLICK to add cells  RCLICK to remove cells", font);
        text.setCharacterSize(17);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::White);
        text.setPosition(0, wSize);
        window.draw(text);

        sf::RectangleShape cell(sf::Vector2f(cellSize - width, cellSize - width));
        //Adding cells
        if (!running) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int x = floor(sf::Mouse::getPosition(window).x / cellSize) * cellSize;
                int y = floor(sf::Mouse::getPosition(window).y / cellSize) * cellSize;
                board[(x / cellSize) + 1][(y / cellSize) + 1] = 1;
            }
        }

        //Remove cells
        if (!running) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                int x = floor(sf::Mouse::getPosition(window).x / cellSize) * cellSize;
                int y = floor(sf::Mouse::getPosition(window).y / cellSize) * cellSize;
                //board[(x / cellSize) + 1][(y / cellSize) + 1] = std::make_tuple(x/cellSize, y/cellSize, 0);
                board[(x / cellSize) + 1][(y / cellSize) + 1] = 0;
            }
        }

        //Draw cells
        cell.setFillColor(sf::Color(0, 0, 255));
        for (int x = 1; x < size+1; x++) {
            for (int y = 1; y < size+1; y++) {
                cell.setPosition(((x - 1) * cellSize) + width, ((y - 1) * cellSize) + width);
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
                    std::cout << "Running" << '\n';
                    running = 1;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
                for (int x = 0; x < size + 2; x++) {
                    for (int y = 0; y < size + 2; y++) {
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

            for (int c = 0; c < size + 2; c++) {
                for (int v = 0; v < size + 2; v++) {
                    tempBoard[c][v] = board[c][v];
                }
            }

            for (int x = 0; x < size + 2; x++) {
                tempBoard[x][0] = 0;
                tempBoard[0][x] = 0;
                tempBoard[x][size + 1] = 0;
                tempBoard[size + 1][x] = 0;
            }

            //if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                for (int k = 1; k < size + 1; k++) {
                    for (int l = 1; l < size + 1; l++) {

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
                for (int c = 0; c < size + 2; c++) {
                    for (int v = 0; v < size + 2; v++) {
                        board[c][v] = tempBoard[c][v];
                    }
                }
           // }
             
        }

        // end the current frame
        window.display();
    }
}
