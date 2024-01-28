//
// Created by jakeb on 11/26/2023.
//

#ifndef MINESWEEPER_WELCOME_H
#define MINESWEEPER_WELCOME_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "TextureManager.h"
#include <string>
#pragma once
using namespace  std;

struct Welcome {
    Welcome();

    int cursorPos;
    int widthPos;
    int numRows;
    int numColumns;
    int numMines;
    float width;
    float height;
    string name;
    sf::Font font;
    sf::Text title;
    sf::Text cursor;
    sf::Text enterName;
    sf::Text typedName;
    void renderWindow();

};


#endif //MINESWEEPER_WELCOME_H
