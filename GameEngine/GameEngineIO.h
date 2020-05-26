#ifndef GAMEENGINEIO_H
#define GAMEENGINEIO_H

#include <string>

#include <fstream>
#include <string>
#include <sstream>

class GameEngine;
class GameEngineIO {
public:
    GameEngineIO(GameEngine* engine);
    ~GameEngineIO();

    void loadGame(std::string fileName);
    void saveGame(std::string fileName);

    void loadPlayers();
    void loadFactories();
    void loadMosaics(Player* players[]);
    void loadStorageArea(Player* players[]);
    void loadBrokenTiles(Player* players[]);
    void loadBag();
    void loadLid();
    void loadSeed();
    
private:
    GameEngine* gameEngine;
    std::vector<std::string> gameInfo;
    bool greyMode;
    bool sixBySixMode;
    bool standard;
    unsigned int noOfPlayers;
    unsigned int noOfFactories;
    unsigned int noOfCentralFactories;
    bool readError;  
};

#endif //GAMEENGINEIO_H