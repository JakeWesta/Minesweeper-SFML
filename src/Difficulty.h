//
// Created by jakeb on 1/24/2024.
//

#ifndef MINESWEEPER_DIFFICULTY_H
#define MINESWEEPER_DIFFICULTY_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "TextureManager.h"
#include <string>
#pragma once
using namespace  std;

class Difficulty {
private:
    string typedName;
    int width;
    int height;
    sf::Font font;
    sf::Text title;
    sf::Sprite fileBox;
    sf::Sprite easyBox;
    sf::Sprite mediumBox;
    sf::Sprite hardBox;
    int numColumns;
    int numRows;
    int numMines;
    bool choiceMade;
public:
    Difficulty();
    void renderWindow();
    Difficulty(string typedName);
};


#endif //MINESWEEPER_DIFFICULTY_H
