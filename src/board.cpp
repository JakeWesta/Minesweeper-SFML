#include "board.h"
#include "tile.h"
#include "leaderboard.h"
#include <random>

void board::renderWindow() {
    this->theClock.restart();
    //Basic pre-game loading functions
    setTiles();
    setMines();
    sf::RenderWindow board(sf::VideoMode((numColumns * 32), (numRows * 32) + 100), "Game");

    while (board.isOpen()) {
        if(!paused){
            //When unpaused, the saved pausedSecs value gets added to elapsed time to continue the clock smoothly after being reset
            secondsRight = theClock.getElapsedTime().asSeconds() + pausedSecs;
        }
        else
            theClock.restart();
        sf::Event event;
        while (board.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                board.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse;
                mouse = sf::Mouse::getPosition(board);
                if (event.mouseButton.button == sf::Mouse::Right) {
                    if(!paused) {
                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {
                                //Condition for when a tile is right-clicked, ensuring that the tile hasn't been selected
                                //already and doesn't have a flag
                                if (!tiles[i][j].hasAFlag() && tiles[i][j].isHidden() &&
                                    tiles[i][j].getTile().getGlobalBounds().contains(board.mapPixelToCoords(mouse))) {
                                    tiles[i][j].assignFlag();

                                    //logic for modifying ones place digit based on whether the counter is negative or not,
                                    //counterLeftNum == 10 would mean it is negative
                                    if (counterLeftNum != 10)
                                        counterRightNum--;
                                    else
                                        counterRightNum++;

                                    //Logic to remove a flag if right-clicked
                                } else if (tiles[i][j].hasAFlag() && tiles[i][j].isHidden() &&
                                           tiles[i][j].getTile().getGlobalBounds().contains(
                                                   board.mapPixelToCoords(mouse))) {
                                    tiles[i][j].deassignFlag();
                                    if (counterLeftNum != 10)
                                        counterRightNum++;
                                    else
                                        counterRightNum--;
                                }
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Left) {
                    //When game is lost or won, the pause button shouldn't be able to be clicked
                    if(!gameLost && !gameWon){
                        if (pause.getGlobalBounds().contains(board.mapPixelToCoords(mouse))) {
                            gamePaused();
                        }
                    }
                    if(!paused) {
                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {
                                if (!tiles[i][j].hasAFlag() && tiles[i][j].isHidden() &&
                                    tiles[i][j].getTile().getGlobalBounds().contains(board.mapPixelToCoords(mouse))) {
                                    //Logic to determine if a tile has a mine, and whether the user lost the game
                                    if(!tiles[i][j].hasAMine())
                                        tiles[i][j].leftClicked();
                                    else
                                        lostGame();
                                }
                            }
                        }
                        //After every click, win condition must be checked
                        checkWin();
                        if (debug.getGlobalBounds().contains(board.mapPixelToCoords(mouse))) {
                            debugged = !debugged;
                        }
                    }
                    if (face.getGlobalBounds().contains(board.mapPixelToCoords(mouse))) {
                        resetBoard();
                    }
                    if (leader.getGlobalBounds().contains(board.mapPixelToCoords(mouse))) {
                        leaderboardOpen = true;
                    }

                }
            }
        }
        //Whenever leaderboard is opened and the game isn't already paused,
        //this pauses it, and before everything is drawn so it is shown when leaderboard window opens
        if(leaderboardOpen && !paused && !gameWon && !gameLost){
            gamePaused();
            theClock.restart();
        }
        board.clear(sf::Color::White);
        //All the necessary logic to keep redrawing each tile with their correct textures,
        //keeping into account things like debug button being pressed
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numColumns; j++){
                tiles[i][j].drawTile(i, j, paused, gameWon, gameLost);
                board.draw(tiles[i][j].getTile());
                if(!paused || gameWon || gameLost) {
                    if (tiles[i][j].hasAFlag())
                        board.draw(tiles[i][j].getFlag());
                    if (tiles[i][j].hasAMine() && !tiles[i][j].isHidden())
                        board.draw(tiles[i][j].getMine());
                    if (!tiles[i][j].isHidden())
                        board.draw(tiles[i][j].getNumber());
                    if (tiles[i][j].hasAMine() && debugged)
                        board.draw(tiles[i][j].getMine());
                }
            }
        }
        clockAndCounter();
        board.draw(face);
        board.draw(pause);
        board.draw(leader);
        board.draw(secRight);
        board.draw(secLeft);
        board.draw(minRight);
        board.draw(minleft);
        board.draw(counterLeft);
        board.draw(counterRight);
        board.draw(counterMiddle);
        board.draw(debug);
        board.display();


        //Condition for when the game is won and the button isn't pressed, creates a person object
        //with the proper time string and int as well as name.
        if(gameWon && leaderboardOpen && !addedName){
            int timeInt = (minutesRight * 60) + (minutesLeft * 600) + secondsLeft * 10 + secondsRight;
            string timeStr = to_string(minutesLeft) + to_string(minutesRight) + ":" + to_string(secondsLeft) +
                             to_string(secondsRight);
            leaderboard leader = leaderboard(18, 24, username ,timeInt ,timeStr);
            leader.renderWindow();
            addedName = true;
            leaderboardOpen = false;

            //Condition for when the button is pressed, opening it normally
        }else if(leaderboardOpen){
            leaderboard leader =leaderboard(18, 24);
            leader.renderWindow();
            leaderboardOpen = false;
        }
    }
}

board::board(float numRows, float numColumns, float numMines, string username){
    this->addedName = false;
    this->username = username;
    gameLost = false;
    gameWon = false;
    tilesRevealed = 0;
    this->debugged = false;
    this->secondsRight = 0;
    this->secondsLeft = 0;
    this->minutesLeft = 0;
    this->minutesRight = 0;

    //Small logic to ensure the correct starting value for the counter
    if(numMines>= 100)
        this->counterMiddleNum = (int(numMines)/10) % 10;
    else
        this->counterMiddleNum =(numMines/10);
    this->counterRightNum = (int(numMines) % 10);
    if(numMines>= 100)
        this->counterLeftNum = (numMines/100);
    else
        this->counterLeftNum = 0;
    this->pausedSecs = 0;
    this->numRows = numRows;
    this->numColumns = numColumns;
    this->numMines = numMines;
    this->paused = false;
    this->leaderboardOpen = false;

    //Loading all textures and setting positions

    sf::Texture* happyface = TextureManager::getTexture("face_happy");
    sf::Texture* digitList = TextureManager::getTexture("digits");
    sf::Texture* debugger = TextureManager::getTexture("debug");
    sf::Texture* leaderBoard = TextureManager::getTexture("leaderboard");
    sf::Texture* pauser = TextureManager::getTexture("pause");


    face.setTexture(*happyface);
    face.setPosition((numColumns/2.0f * 32) -32 ,32 * (numRows +0.5));
    debug.setTexture(*debugger);
    debug.setPosition((numColumns * 32) -304, 32 * (numRows +0.5));
    leader.setTexture(*leaderBoard);
    leader.setPosition((numColumns * 32) -176,32 * (numRows +0.5));
    pause.setTexture(*pauser);
    pause.setPosition((numColumns * 32) -240,32 * (numRows +0.5));


    minleft.setTexture(*digitList);
    minleft.setPosition((numColumns * 32) - 97,32 * (numRows +0.5) + 16);
    minleft.setTextureRect(sf::IntRect(0, 0, 21, 32));
    minRight.setTexture(*digitList);
    minRight.setPosition((numColumns * 32) - 76,32 * (numRows +0.5) + 16);
    minRight.setTextureRect(sf::IntRect(0, 0, 21, 32));
    secLeft.setTexture(*digitList);
    secLeft.setPosition((numColumns * 32) - 54,32 * (numRows +0.5) + 16);
    secLeft.setTextureRect(sf::IntRect(0, 0, 21, 32));

    secRight.setTexture(*digitList);
    secRight.setPosition((numColumns * 32) - 33,32 * (numRows +0.5) + 16);
    secRight.setTextureRect(sf::IntRect(0, 0, 21, 32));

    counterLeft.setTexture(*digitList);
    counterLeft.setPosition(12, 32 * (numRows + 0.5) + 16);
    counterLeft.setTextureRect(sf::IntRect(0, 0, 21, 32));
    counterRight.setTexture(*digitList);
    counterRight.setPosition(54, 32 * (numRows + 0.5) + 16);
    counterRight.setTextureRect(sf::IntRect(0, 0, 21, 32));
    counterMiddle.setTexture(*digitList);
    counterMiddle.setPosition(33, 32 * (numRows + 0.5) + 16);
    counterMiddle.setTextureRect(sf::IntRect(21 * (numMines / 10), 0, 21, 32));
}

void board::clockAndCounter(){
    //All the logic to display the clock and counter digits appropriately
    if(secondsRight > 9){
        theClock.restart();
        pausedSecs = 0;
        secondsLeft++;
    }
    if(secondsLeft >= 6){
        minutesRight++;
        secondsLeft = 0;
    }
    if(minutesRight > 9){
        minutesLeft++;
    }
    if(counterRightNum == -1 && counterMiddleNum != 0){
        counterRightNum  = 9;
        counterMiddleNum --;
    }
    if(counterRightNum == 10 && counterLeftNum != 10){
        counterRightNum  = 0;
        counterMiddleNum ++;
    }
    if(counterRightNum == -1 && counterMiddleNum == 0 && (counterLeftNum != 10 || counterLeftNum != 1)){
        counterRightNum = 1;
        counterMiddleNum = 0;
        counterLeftNum = 10;
    }
    if(counterLeftNum == 10 && counterRightNum == -1 && counterMiddleNum !=0){
        counterMiddleNum--;
        counterRightNum=0;
    }
    if(counterLeftNum == 10 && counterRightNum == 0 && counterMiddleNum ==0){
        counterLeftNum = 0;
        counterRightNum = 0;
    }
    if(counterLeftNum  == 10 && counterRightNum == 10){
        counterMiddleNum++;
        counterRightNum = 0;
    }

    //As the png texture has all digits in a row, the 21 * variable is to determine the displacement,
    //selecting the desired number, as each one is 21 units wide
    secRight.setTextureRect(sf::IntRect(21 * secondsRight, 0, 21, 32));
    secLeft.setTextureRect(sf::IntRect(21 * secondsLeft, 0, 21, 32));
    minRight.setTextureRect(sf::IntRect(21 * minutesRight, 0, 21, 32));
    minleft.setTextureRect(sf::IntRect(21 * minutesLeft, 0, 21, 32));

    counterRight.setTextureRect(sf::IntRect(21 * counterRightNum, 0, 21, 32));
    counterMiddle.setTextureRect(sf::IntRect(21 * counterMiddleNum, 0, 21, 32));
    counterLeft.setTextureRect(sf::IntRect(21 * counterLeftNum, 0, 21, 32));
}


void board::gamePaused(){
    sf::Texture* pauser = TextureManager::getTexture("pause");
    sf::Texture* player = TextureManager::getTexture("play");
    //Switch pause state whenever function is called
    paused = !paused;
    if (paused) {
        pause.setTexture(*player);
        //Saves the ones place second value to have the clock value be uninterrupted when reset (EXPLAIN BETTER)
        pausedSecs = secondsRight;
    } else {
        pause.setTexture(*pauser);
    }

}

void board::lostGame() {

    sf::Texture* loseface = TextureManager::getTexture("face_lose");
    face.setTexture(*loseface);
    gamePaused();
    gameLost = true;

    //Reveals all mines once game is lost
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            if(tiles[i][j].hasAMine()){
                tiles[i][j].reveal();
            }
        }
    }
}



void board::wonGame(){
    sf::Texture* winner = TextureManager::getTexture("face_win");
    face.setTexture(*winner);
    gamePaused();
    gameWon  = true;
    counterRightNum = 0;
    counterLeftNum = 0;
    counterMiddleNum = 0;
    debugged = false;

    //Places flags on all the mines if that hasn't been done by the user
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            if(tiles[i][j].hasAMine()){
                tiles[i][j].assignFlag();
            }
        }
    }
    leaderboardOpen = true;
}

void board::checkWin(){
    //Calculation to determine if every non-mine tile has been revealed
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            if(!tiles[i][j].isHidden() && !tiles[i][j].hasAMine())
                tilesRevealed++;
        }
    }
    if(tilesRevealed == (numColumns * numRows) - numMines)
        wonGame();
    tilesRevealed = 0;
}
void board::resetBoard(){
    //Very similar structure to the constructor, necessary for a game reset
    sf::Texture* happyface = TextureManager::getTexture("face_happy");
    face.setTexture(*happyface);
    sf::Texture* pauser = TextureManager::getTexture("pause");
    pause.setTexture(*pauser);
    minutesLeft = 0;
    secondsLeft = 0;
    secondsRight = 0;
    minutesRight = 0;
    if(numMines>= 100)
        this->counterMiddleNum = (int(numMines)/10) % 10;
    else
        this->counterMiddleNum =(numMines/10);
    this->counterRightNum = (int(numMines) % 10);
    if(numMines>= 100)
        this->counterLeftNum = (numMines/100);
    else
        this->counterLeftNum = 0;
    paused = false;
    pausedSecs = 0;
    gameLost = false;
    gameWon = false;
    addedName = false;

    setTiles();
    setMines();
    theClock.restart();
}
void board::setMines(){
    //Logic to place mines in a random but attempted equally spread out manner
    col.seed(rand());
    row.seed(rand());

    uniform_int_distribution<int> rowRandomizer(0,numRows - 1);
    uniform_int_distribution<int> colRandomizer(0,numColumns - 1);
    for(int i = 0; i < numMines;){
    int rowRand = rowRandomizer(row);
    int colRand = colRandomizer(col);
    if(!tiles[rowRand][colRand].hasAMine()){
        tiles[rowRand][colRand].assignMine();
        i++;
    }
    }
}

void board::setTiles(){
    vector<vector<tile>> tempTiles(numRows, vector<tile>());
    this->tiles = tempTiles;
    //Creates the tiles 2D vector that represents the board grid
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numColumns; j++){
            tile tile;
            tiles[i].push_back(tile);
        }
    }
    //Gives each tile a coordinate for debugging and condition statement purposes
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numColumns; j++){
            tiles[i][j].assignXY(j, i);
        }
    }
    //Gives each tile their 3-8 neighbors, depending on location
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numColumns; j++){
            tiles[i][j].assignNeighbors(tiles);
        }
    }
}