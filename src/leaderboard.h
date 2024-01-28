#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "TextureManager.h"
#include <string>
#include <sstream>
#include <fstream>
#include "players.h"
#include <map>
#pragma once
using namespace  std;


class leaderboard {
private:

    string newName;
    int numRows;
    int numColumns;
    string newTimeStr;
    int newTimeInt;
    string drawnString;
    sf::Text leaderboardNames;
    sf::Text leaderboardTitle;
    sf::Font font;
    map<int, players> nameList;
public:
    leaderboard(int numRows, int numColumns);
    leaderboard(int numRows, int numColumns, string username, int timeInt, string timeStr);
    void renderWindow();
    bool windowOpen();
};

