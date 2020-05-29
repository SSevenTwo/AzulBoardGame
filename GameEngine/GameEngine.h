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
    INVALID_MOVE,
    HELP
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
    void newGame(const std::string playerNames[], const unsigned int noOfPlayers, 
        const unsigned int noOfCentralFactories, const std::string gameMode);
    void loadGameSettings(const unsigned int noOfPlayers, 
        const unsigned int noOfCentralFactories, const std::string gameMode);
    void determineGameMode(const std::string gameMode);
    void determineNoOfPlayersAndFactories(const unsigned int noOfPlayers, 
        const unsigned int noOfCentralFactories);
    void makePlayers(const std::string playerNames[], const std::string gameMode);
    void gameplayLoop(bool& eof, bool& continueMenuLoop);
    bool winConditionMet();
    bool endOfRoundConditionMet();
    void endOfRoundPreparations();
    void endOfGamePreparations();

    //Player turn moves
    int playerTurn(int& result, const std::string playerTurnCommand, int& help);
    std::string interpretPlayerTurn(const int result, int helpNo);
    void swapCurrentTurn();
    void calculatePointsPerRound();
    void calculateEndGamePoints();
    void resetGame();
    std::string help(int help);

    //Player input validation
    void getPlayerInputInLoop(int& result, int& help);
    bool checkInput();
    bool commandsAreValid(std::string commands[], 
        int& factoryNo, Type& tileType, int& storageRow, int& centralFactoryNo);
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
    void moveTilesToMosaicStorage(std::shared_ptr<Player> player, unsigned const int factoryNumber,int centralFactoryNo, unsigned const int row,const Type type);
    void moveTilesToBrokenTiles(std::shared_ptr<Player> player, unsigned const int factoryNumber,int centralFactoryNo, const Type type);
    void moveTilesToLid(std::shared_ptr<Player> player);
    void movePlayerTilesToMosaic();
    void movePlayerTilesToMosaicForGreyMode();
    bool askForMosaicRow(std::shared_ptr<MosaicStorage> playerMosaicStorage, int playerNo, int row);

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
    std::string getGameModeAsString();
    std::vector<std::shared_ptr<Player>> getPlayers();
    std::shared_ptr<Player> getPlayer(int index);
    int getCurrentTurn() const;
    int getPlayerStartingNextRound() const;
    std::shared_ptr<Player> getCurrentPlayer() const;
    std::shared_ptr<Factory> getFactory(unsigned const int number) const;
    std::shared_ptr<LinkedList> getTileBag() const;
    std::shared_ptr<LinkedList> getBoxLid() const;

    //Getters for save functionality
    int getNoOfCentralFactories() const;
    int getNoOfNormalFactories() const;
    int getSeed() const;

    //Setters
    void setPlayer(std::string name, int playerNo, std::string gameMode);
    void setSeed(int seed);
    void setCurrentTurn(int playerNo);
    void setPlayerStartingNextRound(int playerNo);

private:
    std::shared_ptr<GameEngineCallback> gec;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Factory>> factories;
    std::shared_ptr<LinkedList> bag;
    std::shared_ptr<LinkedList> boxLid;
    bool outOfTiles;
    int noOfPlayers;
    int noOfCentralFactories;
    int noOfNormalFactories;
    int currentTurn;
    int playerStartingNextRound;
    int seed;
    
    // Game mode
    bool standardMode;
    bool greyMode;
    bool sixBySixMode;
    bool use2ndFactory;

    // For testing purposes
    bool testing;
    std::vector<std::string> commands;

};

#endif // GAMEENGINE_H