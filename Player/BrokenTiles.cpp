#include "headers/BrokenTiles.h"

BrokenTiles::BrokenTiles(int noOfTiles) {
    this->maxNoOfTiles = noOfTiles;
    this->size = 0;
}

BrokenTiles::~BrokenTiles() {
    clear();
}

std::shared_ptr<Tile>* BrokenTiles::getTiles() {
    return tiles;
}

unsigned int BrokenTiles::calculatePointsLost() {
    unsigned int lostPoints = 0;
    unsigned const int numBrokenTiles = this->size;
    if (numBrokenTiles == 1) {
        lostPoints = 1;
    } else if (numBrokenTiles == 2) {
        lostPoints = 2;
    } else if (numBrokenTiles == 3) {
        lostPoints = 4;
    } else if (numBrokenTiles == 4) {
        lostPoints = 6;
    } else if (numBrokenTiles == 5) {
        lostPoints = 8;
    } else if (numBrokenTiles == 6) {
        lostPoints = 11;
    } else if (numBrokenTiles == 7) {
        lostPoints = 14;
    } else if (numBrokenTiles >= 8) {
        lostPoints = 18;
    }
    return lostPoints;
}

void BrokenTiles::clear() {
    for(int i = 0; i < size; ++i)
        this->tiles[i] = nullptr;
    
    this->size = 0;
}

void BrokenTiles::addTile(std::shared_ptr<Tile> tile) {
    tiles[size] = tile;
    ++size;
}

std::string BrokenTiles::toString() {
    std::string string = "Broken: ";
    for(int i = 0; i < this->size; i++){
        string += tiles[i]->getColourType();
        string += " ";
    }
    for(int i = 0; i < (maxNoOfTiles-this->size); i++){
        string += ".";
        string += " ";
    }

    return string;
}

int BrokenTiles::getSize() const{
    return this->size;
}

int BrokenTiles::getMaxSize() const{
    return this->maxNoOfTiles;
}

std::string BrokenTiles::toSave(){
    std::string string;
    for(int i = 0; i < this->size; i++){
        string += tiles[i]->getColourType();
        string += " ";
    }

    return string;
}
