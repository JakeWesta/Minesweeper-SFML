#include "Welcome.h"
#include "Difficulty.h"

//Function built to center all text elements
void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
}

void Welcome::renderWindow() {
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode((25 * 32), (16 * 32) + 100), "Welcome");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                //Checking if backspace key is pressed
                    if (event.text.unicode == '\b') {
                        //Ignored when no text has been entered
                        if (name.length() == 0) {
                            continue;
                            //Check to ensure that the cursor is also not at the front of the string,
                            //where backspace should have no effect
                        } else if (cursorPos > 0) {
                            //Takes last character off string when cursor is at the rightmost side
                            if (cursorPos == name.length()) {
                                name = name.substr(0, cursorPos - 1);

                            } //Deletes the specific character to the left of the cursor
                            else {
                                name = name.substr(0, cursorPos - 1) + name.substr(cursorPos, name.length() - 1);
                            }
                            cursorPos--;
                            //Decreases the width that is used to calculate where cursor will be shown
                            widthPos -= 5;
                        }
                    }//Logic that ensures an alphanumeric key is capitalized when it's the first string input
                    else if ((name.length() == 0 || cursorPos == 0) && isalpha(static_cast<char>(event.text.unicode))){
                        string upperLetter;
                        upperLetter = toupper(static_cast<char>(event.text.unicode));
                        name = upperLetter + name;
                        cursorPos++;
                        widthPos += 5;

                        //Skips logic for adding lowercase values when at max character length
                    } else if (name.length() == 10) {
                    continue;
                } else if (event.text.unicode < 128 && isalpha(static_cast<char>(event.text.unicode))) {
                        string lowerLetter;
                        lowerLetter = tolower(static_cast<char>(event.text.unicode));
                        name = name.substr(0, cursorPos) + lowerLetter + name.substr(cursorPos, name.length() - 1);
                        cursorPos++;
                        widthPos += 5;
                    }

                //Recalibrating the position that the string is displayed
                typedName.setString(name);
                setText(typedName, width / 2, height / 2 - 45);

                //Logic for ensuring that the cursor is attached to end of string when at rightmost position
                sf::FloatRect textRect = typedName.getLocalBounds();
                if (cursorPos == name.length()) {
                    widthPos = (textRect.width / 2);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                //Logic for keeping track of where the cursor is numerically with cursorPos and the value
                //used for displaying it on screen with widthPos
                if (event.key.code == sf::Keyboard::Left) {
                    if (cursorPos > 0) {
                        widthPos -= 11;
                        cursorPos--;
                    }
                }
                if (event.key.code == sf::Keyboard::Right) {
                    if (cursorPos < name.length()) {
                        widthPos += 11;
                        cursorPos++;
                    }
                }//After at least one character is in the string, the user can press enter to traverse to difficulty selection
                if(event.key.code == sf::Keyboard::Enter && name.length() >= 1){
                    Difficulty difficulty(name);
                    window.close();
                    difficulty.renderWindow();
                }
            }
        }
        sf::Time elapsed1 = clock.getElapsedTime();
        window.clear(sf::Color::Blue);
        window.draw(title);

        //Clock based logic used to create the blinking effect of the cursor
        if(elapsed1.asSeconds() < 0.5){
            setText(cursor, width / 2 + (widthPos), height / 2 - 45);
            window.draw(cursor);
        }else if (elapsed1.asSeconds() > 1){
            clock.restart();
        }
        window.draw(enterName);
        window.draw(typedName);
        window.display();
    }
}

Welcome::Welcome() {
    this->widthPos = 0;
    this->cursorPos = 0;
    this->width = (25 * 32);
    this->height = (16 * 32) + 100;
    font.loadFromFile("files/font.ttf");
    title.setFont(font);
    title.setString("WELCOME TO MINESWEEPER!");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    enterName.setFont(font);
    enterName.setString("Enter your name:");
    enterName.setCharacterSize(20);
    enterName.setFillColor(sf::Color::White);
    enterName.setStyle(sf::Text::Bold);
    typedName.setFillColor(sf::Color::Yellow);
    typedName.setFont(font);
    typedName.setCharacterSize(18);
    typedName.setString(name);
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setFont(font);
    cursor.setString("|");
    cursor.setCharacterSize(18);
    setText(enterName, width / 2, height / 2 - 75);
    setText(title, width / 2, height / 2 - 150);
    setText(typedName, width / 2, height / 2 - 45);
    setText(cursor, width / 2 + (widthPos), height / 2 - 45);
}