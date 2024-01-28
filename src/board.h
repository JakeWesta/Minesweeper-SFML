#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include "tile.h"
#include "TextureManager.h"
#pragma once
using namespace  std;

class board {
private:
    float numRows;
    float numColumns;
    float numMines;
    bool paused;
    int tilesRevealed;
    vector<vector<tile>> tiles;
    sf::Sprite debug;
    sf::Sprite face;
    sf::Sprite leader;
    sf::Sprite pause;
    sf::Sprite play;
    sf::Sprite counterLeft;
    sf::Sprite counterRight;
    sf::Sprite counterMiddle;
    sf::Sprite minleft;
    sf::Sprite minRight;
    sf::Sprite secLeft;
    sf::Sprite secRight;
    bool debugged;
    bool addedName;
    int secondsRight;
    int secondsLeft;
    int minutesLeft;
    int minutesRight;
    int counterMiddleNum;
    int counterRightNum;
    int counterLeftNum;
    int pausedSecs;
    bool gameWon;
    bool gameLost;
    mt19937 col;
    mt19937 row;
    sf::Clock theClock;
    string username;
    bool leaderboardOpen;


public:
    board(float numRows, float numColumns, float numMines, string username);
    void setText();
    board();
    void renderWindow();
    void lostGame();
    void clockAndCounter();
    void gamePaused();
    void setMines();
    void setTiles();
    void resetBoard();
    void wonGame();
    void checkWin();
};
