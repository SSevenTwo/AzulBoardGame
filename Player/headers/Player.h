#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include "MosaicStorage.h"

class Player{
public:
    Player(std::string name, int playerNo, std::string gameMode);
    ~Player();
    int getPoints();
    void setPoints(int points);
    std::string getName();
    int getPlayerNo();
    std::shared_ptr<MosaicStorage> getMosaicStorage();
    void determineGameMode(std::string gameMode);
    
private:
    int points;
    int playerNo;
    std::string name;
    std::shared_ptr<MosaicStorage> mosaicStorage;
    bool greyMode;
    bool sixBySixMode;
};

#endif // PLAYER_H