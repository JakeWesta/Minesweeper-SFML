#include "leaderboard.h"
void setTexts(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

//Logic for when button is pressed, not on a win
leaderboard::leaderboard(int numRows, int numColumns) {
    this->numRows = numRows;
    this->numColumns = numColumns;
    this->drawnString = "";
    ifstream leaderboardReading("files/leaderboard.txt");
    string oneLine;
    int i = 1;

    //While loop that goes one line at a time until the end of the file,
    //with the intention of making a map of the player object keyed to their index
    while(getline(leaderboardReading, oneLine)){
        string timeStr;
        string name;
        istringstream stream(oneLine);
        getline(stream, timeStr, ',');
        getline(stream, name, ',');

        //After the initial post-win leaderboard opening with starred name, it gets removed from the name
        name.erase(remove(name.begin(), name.end(), '*'), name.end());
        players player(timeStr, name);
        nameList.emplace(i, player);
        i++;
    }
    auto iter = nameList.begin();
    //Puts together the whole map to create what will be displayed on the window
    for(; iter!= nameList.end(); ++iter) {
        drawnString += to_string(iter->first) + ".\t" + nameList[iter->first].getTimeStr() + "\t" + nameList[iter->first].getName() + "\n\n";
    }

}

//Logic for when win occurs, not button press
leaderboard::leaderboard(int numRows, int numColumns, string username, int timeInt, string timeStr) {
    this->numRows = numRows;
    this->numColumns = numColumns;
    this->newName = username;
    this->newTimeStr = timeStr;
    this->newTimeInt = timeInt;
    this->drawnString = "";
    ifstream leaderboardReading("files/leaderboard.txt");
    string oneLine;
    int i = 1;

    while(getline(leaderboardReading, oneLine)){
        string timeStr;
        string name;
        istringstream stream(oneLine);
        getline(stream, timeStr, ',');
        getline(stream, name, ',');
        name.erase(remove(name.begin(), name.end(), '*'), name.end());
        players player(timeStr, name);
        nameList.emplace(i, player);
        i++;
    }

    //Logic to check if the winner has a faster time than anyone on the leaderboard
    i = 0;
    int pos = nameList.size() + 1;
    auto iter = nameList.begin();
    map<int, players> tempMap;
    bool nameAdded = false;
    iter = nameList.begin();

    for(; iter!= nameList.end(); ++iter) {
        if(i==5)
            //Player did not beat any times
            break;

        if(pos < nameList.size() + 1 && i != 5){
            //After the pos of the new record-breaking player is determined,
            //adds the succeeding players. This is for when the player is anything but 5th place,
            //as other player ranks following need to be updated.
            tempMap.emplace(iter->first + 1,iter->second);

            //Checking if new player time is better than rhs, with the 0.01 added
            //in the case of a tie.
        }else if(float(newTimeInt) < float(nameList[iter->first].getTimeInt() + 0.01)){
            // pos is updated to new value if player time is faster
            pos = iter->first;

            //New player object is created with asterisk added
            newName += "*";
            players newPlayer(newTimeStr, newName);
            tempMap.emplace(iter->first, newPlayer);

            //Checks to see if compared rhs should be added behind new player,
            //only false when pos is 5
            if(iter->first+1 <= 5){
                i++;
                tempMap.emplace(iter->first+1, iter->second);
            }
            nameAdded = true;
        } else{
            //Base case when player hasn't beat a time, players are copied over
            tempMap[iter->first] = nameList[iter->first];
        }
        i++;
    }

    //Edge case for when leaderboard isn't completely filled
    if(!nameAdded && nameList.size() < 5) {
        newName += "*";
        players newPlayer(newTimeStr, newName);
        tempMap.emplace(nameList.size() + 1, newPlayer);
    }
    nameList = tempMap;

    //Rewriting leaderboard.txt with new player, if there is one
    ofstream leaderboardWriting("files/leaderboard.txt");
    iter = nameList.begin();
    for(; iter!= nameList.end(); ++iter) {
        leaderboardWriting << nameList[iter->first].getTimeStr() + "," + nameList[iter->first].getName() << endl;
        drawnString += to_string(iter->first) + ".\t" + nameList[iter->first].getTimeStr() + "\t" + nameList[iter->first].getName() + "\n\n";
    }
}

void leaderboard::renderWindow() {
    font.loadFromFile("files/font.ttf");
    leaderboardTitle.setFont(font);
    leaderboardTitle.setString("LEADERBOARD");
    leaderboardTitle.setCharacterSize(20);
    leaderboardTitle.setFillColor(sf::Color::White);
    leaderboardTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
    leaderboardNames.setString(drawnString);
    leaderboardNames.setFont(font);

    leaderboardNames.setCharacterSize(18);
    leaderboardNames.setFillColor(sf::Color::White);
    leaderboardNames.setStyle(sf::Text::Bold);
    int width = (numColumns * 16);
    int height = (numRows * 16) + 50;
    setTexts(leaderboardNames, width / 2, height/ 2 +20);
    setTexts(leaderboardTitle, width / 2, height/ 2 -100);

    sf::RenderWindow window(sf::VideoMode(width, (numRows * 16) + 50), "Leaderboard");
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(leaderboardTitle);
        window.draw(leaderboardNames);
        window.display();
    }
}