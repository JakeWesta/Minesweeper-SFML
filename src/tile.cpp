#include "tile.h"
tile::tile() {
    hasFlag = false;
    hasMine = false;
    hidden = true;
    gameLost = false;
    neighborMines = 0;
}

void tile::leftClicked() {
    //Reveals tile
    this->hidden = false;
    //Calls function ti
    checkNeighbors();
    if(neighborMines == 0){
        for(int i = 0; i < neighbor.size(); i++){
                if(neighbor.at(i)->isHidden()){
                    //Recursively calling
                    neighbor.at(i)->leftClicked();
                }
        }
    }
    if(hasFlag)
        this->hidden = true;
    if(hasMine) {
        this->hidden = true;
    }
}

bool tile::isHidden(){
    if(hidden){
        return true;
    }
    return false;
}

void tile::assignMine() {
        this->hasMine = true;
}
bool tile::hasAMine(){
    if (hasMine){
        return true;
    }
    return false;
}
bool tile::hasAFlag(){
    if (hasFlag){
        return true;
    }
    return false;
}
void tile::assignFlag() {
    this->hasFlag = true;
}

void tile::deassignFlag() {
    this->hasFlag = false;
}

void tile::drawTile(int col, int row, bool paused, bool gameWon, bool gameLost) {
    sf::Texture* tileHidden = TextureManager::getTexture("tile_hidden");
    sf::Texture* flags = TextureManager::getTexture("flag");
    sf::Texture* mines = TextureManager::getTexture("mine");
    sf::Texture* tileRevealed = TextureManager::getTexture("tile_revealed");
    vector<sf::Texture*> numbers;
    for(int i = 1; i < 9; i++){
        string textureName = "number_" + to_string(i);
        sf::Texture* number = TextureManager::getTexture(textureName);
        numbers.push_back(number);
    }
    if(hidden){
        tileSprite.setTexture(*tileHidden);
        tileSprite.setOrigin(tileHidden->getSize().x / 2.0f, tileHidden->getSize().y / 2.0f);
        tileSprite.setPosition((tileHidden->getSize().y/ 2.0f)+(tileHidden->getSize().y) * row,(tileHidden->getSize().x/ 2.0f) +(tileHidden->getSize().x) * col);
    }else if(!hidden && (!hasFlag || gameLost)){
        tileSprite.setTexture(*tileRevealed);
        tileSprite.setOrigin(tileRevealed->getSize().x / 2.0f, tileRevealed->getSize().y / 2.0f);
        tileSprite.setPosition((tileRevealed->getSize().y/ 2.0f)+(tileRevealed->getSize().y) * row,(tileHidden->getSize().x/ 2.0f) +(tileHidden->getSize().x) * col);
        if(neighborMines != 0 && !hasMine){
            number.setTexture(*numbers.at(neighborMines-1));
            number.setOrigin(numbers.at(neighborMines-1)->getSize().x / 2.0f, numbers.at(neighborMines-1)->getSize().y / 2.0f);
            number.setPosition((tileRevealed->getSize().y/ 2.0f)+(tileRevealed->getSize().y) * row,(tileHidden->getSize().x/ 2.0f) +(tileHidden->getSize().x) * col);
        }
    }
     flag.setTexture(*flags);
     flag.setOrigin(flags->getSize().x / 2.0f, flags->getSize().y / 2.0f);
     flag.setPosition((tileHidden->getSize().y/ 2.0f)+(tileHidden->getSize().y) * row,(tileHidden->getSize().x/ 2.0f) +(tileHidden->getSize().x) * col);
     if(hasMine){
         mine.setTexture(*mines);
         mine.setOrigin(mines->getSize().x / 2.0f, mines->getSize().y / 2.0f);
         mine.setPosition((tileHidden->getSize().y/ 2.0f)+(tileHidden->getSize().y) * row,(tileHidden->getSize().x/ 2.0f) +(tileHidden->getSize().x) * col);
     }
    if(paused && !gameWon && !gameLost)
        tileSprite.setTexture(*tileRevealed);

}

//Getter functions
sf::Sprite tile::getTile(){
    return tileSprite;
}
sf::Sprite tile::getNumber(){
    return number;
}
sf::Sprite tile::getFlag(){
    return flag;
}
sf::Sprite tile::getMine(){
    return mine;
}

void tile::assignNeighbors(vector<vector<tile>>& neighbors) {
    //Nested for loop for checking in an index grid around the tile
    for(int i = -1; i < 2; i++){
        for( int j = -1; j < 2; j++) {
            //try catch block for edge tiles that don't have all 8 neighbors and would go out of range
            try {
                //Skips the tile calling the function
                if(i == 0 && j == 0) {continue;}
                else{
                    tile *adjacent = &neighbors.at(y + i).at(x + j);
                    neighbor.push_back(adjacent);
                }
            } catch (out_of_range &e) {
            }
        }
    }
}

//
void tile::checkNeighbors(){
    for(int i = 0; i < neighbor.size(); i++){
        if(neighbor.at(i) != nullptr && neighbor.at(i)->hasMine){
            neighborMines++;
        }
    }
}

//Function to keep track of each tile coordinate
void tile::assignXY(int x, int y) {
    this->x = x;
    this->y = y;
}

//Called when game is lost to reveal all
void tile::reveal() {
    this->hidden = false;
    this->gameLost = true;
}

////Debugging function
//void tile::displayNeighbors(){
//    for(int i = 0; i < neighbor.size() ; i++){
//        neighbor.at(i)->printXY();
//    }
//}


