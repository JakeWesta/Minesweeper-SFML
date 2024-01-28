
#include "players.h"

players::players(string time, string name) {
    this->timeStr = time;
    this->name = name;
}
string players::getName(){
    return name;
}

int players::getTimeInt() {
    //Dividing the provided time in "00:00" format into minutes and seconds
    string minutes = timeStr.substr(0, 2);
    string seconds = timeStr.substr(3, 2);
    //converting the strings to an integer of total seconds
    timeInt = stoi(minutes) * 60 + stoi(seconds);
    return timeInt;
}

players::players() {
}

string players::getTimeStr() {
    return timeStr;
}
