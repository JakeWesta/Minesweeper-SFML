#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "TextureManager.h"
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#pragma once
using namespace  std;


class players {
private:
    string name;
    string timeStr;
    int timeInt;
public:
    players();
    players(string time, string name);
    int getTimeInt();
    string getName();
    string getTimeStr();
};


