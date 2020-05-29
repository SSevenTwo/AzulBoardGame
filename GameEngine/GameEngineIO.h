#ifndef GAMEENGINEIO_H
#define GAMEENGINEIO_H

#include <string>
#include <fstream>
#include <string>
#include <sstream>

#define LEGACY_SAVE 36

class GameEngine;
class GameEngineIO {
public:
    GameEngineIO(GameEngine* engine);
    ~GameEngineIO();

    void loadGame(std::string fileName);
    void saveGame(std::string fileName, std::string gameMode, int noOfPlayers);

    void loadPlayers();
    void loadFactories();
    void loadMosaics();
    void loadStorageArea();
    void loadBrokenTiles();
    void loadBag();
    void loadLid();
    void loadSeed();
    void determineGameMode(std::string gameMode);
    void determineNoOfPlayers(std::string playerNo);
    void determineNoOfCentralFactories(std::string noOfCentralFactories);
    void setLegacySaveSettings();
    
private:
    GameEngine* gameEngine;
    std::vector<std::string> gameInfo;
    int index;
    bool greyMode;
    bool sixBySixMode;
    bool standard;
    bool legacySave;
    unsigned int noOfPlayers;
    unsigned int noOfFactories;
    unsigned int noOfStorageRows;
    unsigned int noOfCentralFactories;
    unsigned int noOfTilesPerFactory;
    bool readError;  
};

#endif //GAMEENGINEIO_H