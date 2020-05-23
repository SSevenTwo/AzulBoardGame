#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "../Components/headers/LinkedList.h"
#include "../Components/headers/Factory.h"
#include "../Player/headers/Player.h"
#include "GameEngineCallback.h"

#include <fstream>
#include <string>
#include <sstream>
#include<vector>

#define NUM_NORMAL_FACTORIES    5
#define NUM_PLAYERS             2
#define NUM_FACTORIES           6
#define MAX_ROWS                5

enum Error_Message{
    INVALID_COMMAND,
    SUCCESS,
    SAVED,
    NO_TILES_IN_CENTRAL,
    COLOUR_NOT_IN_FACTORY,
    INVALID_MOVE
};

class GameEngineIO;
class GameEngine{
public:
    GameEngine();
    GameEngine(const int seed);
    ~GameEngine();
    void commonGameEngine();
    void instantiateFactories();

    //Gameplay Conditions
    void newGame(const std::string playerNames[], int noOfPlayers, int noOfFactories, std::string gameMode);
    void determineGameMode(std::string gameMode);
    void gameplayLoop(bool& eof, bool& continueMenuLoop);
    bool winConditionMet();
    bool endOfRoundConditionMet();
    void endOfRoundPreparations();

    //Player turn moves
    int playerTurn(const std::string playerTurnCommand);
    std::string interpretPlayerTurn(const int result);
    void swapCurrentTurn();
    void calculatePointsPerRound();
    void calculateEndGamePoints();
    void resetGame();

    //Player input validation
    bool checkInput();
    bool checkCommand1(const std::string input, int& factoryNo);
    bool checkCommand2(const std::string input, Type& tileType);
    bool checkCommand3(const std::string input, int& storageRow);
    bool checkCommand4(const std::string input, int& centralFactoryNo, int factoryNo);
    bool tileExistsInFactory(Type tileType, int factoryNo);
    bool centralFactoryOnlyHasFirstTile(int centralFactoryNo);
    bool inputIsInt(const std::string input);
    bool normalFactoriesAreEmpty();

    //Moving of tiles
    bool moveTilesFromFactory(std::shared_ptr<Player> player,unsigned const int factoryNumber,int centralFactoryNo,unsigned const int row, const Type type, const bool toBroken);
    void returnFactoryTilesByColour(Type type);
    void moveTilesToMosaicStorage(std::shared_ptr<Player> player, unsigned const int factoryNumber,int centralFactoryNo, unsigned const int row,const Type type);
    void moveTilesToBrokenTiles(std::shared_ptr<Player> player, unsigned const int factoryNumber, const Type type);
    void moveTilesToLid(std::shared_ptr<Player> player);
    void movePlayerTilesToMosaic();

    //Component Utilities
    void populateFactories();
    void populateBagAndShuffle();
    void shuffle(std::shared_ptr<Tile> bagToShuffle[] , unsigned const int size);
    void addTilesByColourToBag(const Type type, std::shared_ptr<Tile> bagToShuffle[], int &bagCount);
    void clearBoxLid();
    void refillBag();
    bool changeType(Type& tileType, char tileChar);
    void removeOtherFirstPlayerToken(int centralFactoryNo);
    
    //Getters
    int getCurrentTurn() const;
    int getPlayerStartingNextRound() const;
    std::shared_ptr<Player> getCurrentPlayer() const;
    Player* getPlayerOne() const;
    Player* getPlayerTwo() const;
    std::shared_ptr<Factory> getFactory(unsigned const int number) const;
    LinkedList* getTileBag() const;
    LinkedList* getBoxLid() const;
    int getSeed() const;

    //Setters
    void setPlayerOne(std::string player);
    void setPlayerTwo(std::string player);
    void setSeed(int seed);
    void setCurrentTurn(int playerNo);
    void setPlayerStartingNextRound(int playerNo);

private:
    std::vector<std::shared_ptr<Player>> players;
    int noOfPlayers;
    int noOfCentralFactories;
    bool use2ndFactory;
    std::vector<std::shared_ptr<Factory>> factories;
    LinkedList* bag;
    LinkedList* boxLid;
    int currentTurn;
    int playerStartingNextRound;
    GameEngineCallback* gec;
    int seed;
    std::string gameInfo[36];
    
    // Game mode
    bool standardMode;
    bool greyMode;
    bool sixBySixMode;

    // For testing purposes
    bool testing;

};

#endif // GAMEENGINE_H