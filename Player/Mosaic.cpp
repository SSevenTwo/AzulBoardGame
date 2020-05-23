#include "headers/Mosaic.h"

#include <iostream>

Mosaic::Mosaic(std::string gameMode){
    determineGameMode(gameMode);

    // Making 5x5 2d array for tiles
    this->grid = new std::shared_ptr<Tile>*[maxNoRows];
    for (unsigned int row = 0; row < maxNoRows; ++row) {
        grid[row] = new std::shared_ptr<Tile>[maxNoCols];
    }

    this->noOfBlacks = 0;
    this->noOfYellows = 0;
    this->noOfDarkBlues = 0;
    this->noOfLightBlues = 0;
    this->noOfReds = 0;
    this->noOfOranges = 0;
    this->pointsThisRound = 0;

}

Mosaic::~Mosaic(){
    // delete tiles
    for (unsigned int row = 0; row < maxNoRows; ++row) {
        for (unsigned int col = 0; col < maxNoCols; ++col) {
            if(grid[row][col] != nullptr){
                grid[row][col] = nullptr;
            }
        }
    }

    this->grid = nullptr;
    this->pointsThisRound = 0;
}

std::shared_ptr<Tile>** Mosaic::getGrid(){
    return this->grid;
}

bool Mosaic::findFullRow(){
    bool fullRowFound = false;
    int counter = 0;

    for (unsigned int row = 0; row < maxNoRows; ++row) {
        for (unsigned int col = 0; col < maxNoCols; ++col) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == MAX_NO_TILES){
                    fullRowFound = true;
                }
            }
        }
        counter = 0;
    }

    return fullRowFound;
}

bool Mosaic::isRowFull(unsigned const int row) {
    unsigned int i = 0;
    bool isFull = false;
    for (unsigned int counter = 0; counter < maxNoRows; ++counter) {
        if (grid[row][i] != nullptr) {
            ++i;
        }
    }
    if (i==maxNoRows) {
        isFull = true;
    }
    return isFull;
}

bool Mosaic::findFullCol(unsigned int col){
    bool colIsFull = false;
    int counter = 0;

    for (unsigned int row = 0; row < maxNoRows; ++row) {
        if(grid[row][col] != nullptr){
            ++counter;
            if(counter == MAX_NO_TILES){
                colIsFull = true;
            }
        }
    }

    return colIsFull;
}

int Mosaic::noOfFiveColours(){
    int timesGotAll5Colours = 0;

    if(this->noOfBlacks == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfReds == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfDarkBlues == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfLightBlues == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfYellows == MAX_NO_TILES)
        ++timesGotAll5Colours;

    return timesGotAll5Colours;
}

void Mosaic::numberOfSequentialTiles(unsigned int row, unsigned int col){
    int numSequential = 0;
    if (checkSequentialCols(row, col)) {
        ++numSequential;
    }
    if (checkSequentialRows(row,col)) {
        ++numSequential;
    }

    if (numSequential == 2) {
        pointsThisRound += 2;
    } else {
        pointsThisRound += 1;
    }
}

bool Mosaic::checkSequentialRows(int row, int col){
    bool hasSequential = false;
    int numberOfSequentialTiles = 0;
    bool sequential = true;

    // Check all values before the row
    for (int i = row-1; i >= 0; --i) {
        if (sequential && grid[i][col] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    sequential =  true;

    // Check all values after the row
    for (unsigned int i = row+1; i < maxNoRows; ++i) {
        if (sequential && grid[i][col] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    pointsThisRound += numberOfSequentialTiles;

    return hasSequential;

}

bool Mosaic::checkSequentialCols(int row, int col){
    bool hasSequential = false;
    int numberOfSequentialTiles = 0;
    bool sequential = true;
    // Check all values before the row
    for (int i = col-1; i >= 0; --i) {
        if (sequential && grid[row][i] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    sequential = true;

    // Check all values after the row
    for (unsigned int i = col+1; i < maxNoCols; ++i) { 
        if (sequential && grid[row][i] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    pointsThisRound += numberOfSequentialTiles;

    return hasSequential;
}

int Mosaic::getPointsThisRound(){
    return this->pointsThisRound;
}

void Mosaic::resetPoints(){
    this->pointsThisRound = 0;
}

bool Mosaic::isSpaceFree(unsigned int row, unsigned int col){
    bool valid = false;
    if(this->grid[row][col] == nullptr){
        valid = true;
    }
    return valid;
}

bool Mosaic::addTile(std::shared_ptr<Tile> tile, unsigned int row, unsigned int col){
    bool added = false;
    if(col >= 0 && col < maxNoCols && row >=0 && row < maxNoRows){
        if(grid[row][col] == nullptr){
            this->grid[row][col] = tile;
            numberOfSequentialTiles(row,col);
            incrementColorCounter(tile->getType());
            added = true;
        }
    }

    return added;
}

void Mosaic::incrementColorCounter(Type tileType){
     if(tileType == Type::RED)
        this->noOfReds++;
    if(tileType == Type::BLACK)
        this->noOfBlacks++;
    if(tileType == Type::DARK_BLUE)
        this->noOfDarkBlues++;
    if(tileType == Type::LIGHT_BLUE)
        this->noOfLightBlues++;
    if(tileType == Type::YELLOW)
        this->noOfYellows++;
}

int Mosaic::getColourColumn(unsigned const int row, unsigned const int colour) {
    int toReturn = -1;
    if (row >= 0 && row < maxNoRows && colour >= 0 && colour < maxNoCols) {
        toReturn = colourColumns[row][colour];
    } 
    return toReturn;
}

std::string Mosaic::rowToString(int index){
    std::string string = "|| ";

    for(unsigned int i = 0; i< maxNoCols; ++i){
        if(this->grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += ". ";
        }
       
    }

    return string;
}

std::string Mosaic::templateRowToString(int index) {
    std::string string = "";

    for(unsigned int i = 0; i< maxNoCols; ++i){
        string += colourGrid[index][i];
        string += " ";
       
    }

    return string;
}

std::string Mosaic::rowToSave(int index){
    std::string string;
    for(unsigned int i = 0; i< maxNoCols; ++i){
        if(this->grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += toSavecolourGrid[index][i];
            string += " ";
        }      
    }
    return string;
}

int Mosaic::calculateEndGamePoints() {
    int endGamePoints = 0;


    endGamePoints += 2*numFullRows() + 7*numFullCols() + 10*noOfFiveColours();

    return endGamePoints;
}

int Mosaic::numFullCols() {
    int counter = 0;
    int numCols = 0;

    for (unsigned int col = 0; col < maxNoCols; ++col) {
        for (unsigned int row = 0; row < maxNoRows; ++row) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == MAX_NO_TILES){
                    ++numCols;
                }
            }
        }
        counter = 0;
    }

    return numCols;
}

int Mosaic::numFullRows() {
    int counter = 0;
    int numRows = 0;

    for (unsigned int row = 0; row < maxNoRows; ++row) {
        for (unsigned int col = 0; col < maxNoCols; ++col) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == MAX_NO_TILES){
                    ++numRows;
                }
            }
        }
        counter = 0;
    }

    return numRows;
}

void Mosaic::determineGameMode(std::string gameMode){
    if(gameMode == "grey"){
        this->greyMode = true;
        this->sixBySixMode = false;
        this->maxNoRows = 5;
        this->maxNoCols = 5;
    } else if(gameMode == "six"){
        this->greyMode = false;
        this->sixBySixMode = true;
        this->maxNoRows = 6;
        this->maxNoCols = 6;
    }else{
        this->greyMode = false;
        this->sixBySixMode = false;
        this->maxNoRows = 5;
        this->maxNoCols = 5;
    }

    
}

bool Mosaic::colourExistsInCol(Type type, unsigned int col){
    bool exists = false;
    for(unsigned int row = 0; row < maxNoRows; ++row){
        if(this->grid[row][col] != nullptr){
            if(this->grid[row][col]->getType() == type){
                exists = true;
            }
        }
    }

    return exists;
}

bool Mosaic::colourExistsInRow(Type type, unsigned int row){
    bool exists = false;
    for(unsigned int col = 0; col < maxNoCols; ++col){
        if(this->grid[row][col]!= nullptr){
            if(this->grid[row][col]->getType() == type){
                exists = true;
            }
        }
    }

    return exists;
}