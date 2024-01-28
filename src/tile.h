#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "TextureManager.h"
#include <string>
#pragma once
using namespace  std;

class tile {
private:
    bool hasMine;
    bool hasFlag;
    bool hidden;
    bool gameLost;
    int x;
    int y;
    sf::Sprite tileSprite;
    sf::Sprite flag;
    sf::Sprite mine;
    sf::Sprite number;
    vector<tile*> neighbor;
    int neighborMines;

public:
    tile();
    void leftClicked();
    void assignMine();
    void assignFlag();
    void deassignFlag();
    void drawTile(int col, int row, bool paused, bool gameWon, bool gameLost);
    sf::Sprite getSprite();
    sf::Sprite getTile();
    sf::Sprite getFlag();
    sf::Sprite getMine();
    bool isHidden();
    void assignXY(int x, int y);
    void printXY();
    bool hasAFlag();
    void assignNeighbors(vector<vector<tile>>& neighbors);
    bool hasAMine();
    void checkNeighbors();
    sf::Sprite getNumber();
    void reveal();
    void displayNeighbors();
};
