#include "Difficulty.h"
#include "board.h"

void setTexta(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

Difficulty::Difficulty() {
}

void Difficulty::renderWindow() {
    sf::RenderWindow window(sf::VideoMode((25 * 32), (16 * 32) + 100), "Difficulty");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                //Necessary initiation of mouse vector to be able to check where clicks occur
                sf::Vector2i mouse;
                mouse = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    //Reads from file when box is clicked
                    if(fileBox.getGlobalBounds().contains(window.mapPixelToCoords(mouse))){
                        fstream boardFile("files/board_config.cfg", ios_base::in);
                        string strnumColumns;
                        string strnumRows;
                        string strnumMines;
                        getline(boardFile, strnumColumns);
                        this->numColumns = stoi(strnumColumns);
                        getline(boardFile, strnumRows);
                        this->numRows = stoi(strnumRows);
                        getline(boardFile, strnumMines);
                        this->numMines = stoi(strnumMines);
                        choiceMade = true;

                        //Following logic determines which difficulty box is clicked, and gives predetermined values
                        //to numRows, numColumns, and numMines depending on which
                    } else if(easyBox.getGlobalBounds().contains(window.mapPixelToCoords(mouse))){
                        this->numColumns = 22;
                        this->numRows = 14;
                        this->numMines = 25;
                        choiceMade = true;
                    }else if(mediumBox.getGlobalBounds().contains(window.mapPixelToCoords(mouse))){
                        this->numColumns = 26;
                        this->numRows = 16;
                        this->numMines = 50;
                        choiceMade = true;
                    }else if(hardBox.getGlobalBounds().contains(window.mapPixelToCoords(mouse))){
                        this->numColumns = 32;
                        this->numRows = 20;
                        this->numMines = 99;
                        choiceMade = true;
                    }
                }
            }
        }
        if(choiceMade){
            //Opens up game with provided numRows, numColumns, etc. that was determined by user click
            board game(numRows, numColumns, numMines, typedName);
            window.close();
            game.renderWindow();
        }

        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(easyBox);
        window.draw(mediumBox);
        window.draw(fileBox);
        window.draw(hardBox);
        window.display();
    }
}

Difficulty::Difficulty(string typedName) {
    this->typedName = typedName;
    this->width = (25 * 32);
    this->height = (16 * 32) + 100;
    this->choiceMade = false;

    font.loadFromFile("files/font.ttf");
    title.setFont(font);
    title.setString("CHOOSE YOUR DIFFICULTY, " + typedName);
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Texture* fromfile = TextureManager::getTexture("fromfile");
    fileBox.setTexture(*fromfile);
    fileBox.setPosition(width/2 - fileBox.getGlobalBounds().width/2 ,150);

    sf::Texture* easy = TextureManager::getTexture("easy");
    easyBox.setTexture(*easy);
    easyBox.setPosition(width/2- easyBox.getGlobalBounds().width/2,250);

    sf::Texture* medium = TextureManager::getTexture("medium");
    mediumBox.setTexture(*medium);
    mediumBox.setPosition(width/2- mediumBox.getGlobalBounds().width/2,350);

    sf::Texture* hard = TextureManager::getTexture("hard");
    hardBox.setTexture(*hard);
    hardBox.setPosition(width/2- hardBox.getGlobalBounds().width/2,450);

    setTexta(title, width / 2, height / 2 - 220);
}
