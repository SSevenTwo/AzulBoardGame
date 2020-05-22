#include "headers/Player.h"

Player::Player(std::string name, int playerNo){
    this->points = 0;
    this->mosaicStorage = std::make_shared<MosaicStorage>();
    this->name = name;
    this->playerNo = playerNo;
}

Player::~Player(){
}

std::shared_ptr<MosaicStorage> Player::getMosaicStorage(){
    return this->mosaicStorage;
}

int Player::getPoints(){
    return this->points;
}

int Player::getPlayerNo(){
    return this->playerNo;
}

void Player::setPoints(int points){
    this->points = points;
}

std::string Player::getName(){
    return this->name;
}

