#include "headers/Player.h"
#include <iostream>

Player::Player(std::string name, int playerNo, std::string gameMode){
    this->points = 0;
    this->mosaicStorage = std::make_shared<MosaicStorage>(gameMode);
    this->name = name;
    this->playerNo = playerNo;
    determineGameMode(gameMode);
}

Player::~Player(){
}

void Player::determineGameMode(std::string gameMode){
    if(gameMode == "grey"){
        this->greyMode = true;
        this->sixBySixMode = false;
        std::cout << "GREY ACTIVATED PLAYER DEPLOYED" << std::endl;
    } else if(gameMode == "six"){
        this->greyMode = false;
        this->sixBySixMode = true;
        std::cout << "SIX ACTIVATED PLAYER DEPLOYED" << std::endl;
    }else{
        this->greyMode = false;
        this->sixBySixMode = false;
        std::cout << "STANDARD ACTIVATED PLAYER DEPLOYED" << std::endl;
    }
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

