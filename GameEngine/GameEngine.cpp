#include "GameEngine.h"
#include "GameEngineIO.h"
#include <random>
#include "../Input.h"
#include <memory>


GameEngine::GameEngine() {
    commonGameEngine();
    this->testing = false;
}

GameEngine::GameEngine(const int seed) {
    commonGameEngine();
    this->seed = seed;
    this->testing = true;
}

void GameEngine::commonGameEngine(){
    gec = new GameEngineCallback();

    // for(int i = 0; i<NUM_FACTORIES; ++i){
    //     factory[i] = new Factory();
    // }

    this->bag = new LinkedList();
    this->boxLid = new LinkedList();

    this->currentTurn = 0;
    this->playerStartingNextRound = 0;
    this->noOfPlayers = 0;
    this->noOfCentralFactories = 0;
    this->use2ndFactory = false;
    this->greyMode = false;
    this->sixBySixMode = false;
    this->standardMode = true;
}

void GameEngine::instantiateFactories(){
    // int noOfFactories = 5;

    // if(this->sixBySixMode){
    //     noOfFactories = 6;
    // }

    for(int i = 0; i<noOfCentralFactories; ++i){
        std::shared_ptr<Factory> factory = std::make_shared<Factory>();
        this->factories.push_back(factory);
    }

    for(int i = 0; i < 5; ++i){
        std::shared_ptr<Factory> factory = std::make_shared<Factory>();
        this->factories.push_back(factory);
    }
}

GameEngine::~GameEngine() {

    delete gec;
    delete bag;   
    delete boxLid;

    if(factories.size()>0){
        for(int i = 0;  i < (noOfCentralFactories + NUM_NORMAL_FACTORIES); i++){
            if(factories[i] != nullptr){
                factories[i] = nullptr;
            }
        }
    }

}

//setters
void GameEngine::setPlayerOne(std::string player){
    //this->playerOne = new Player(player);
}
void GameEngine::setPlayerTwo(std::string player){
    //this->playerTwo = new Player(player);
}

void GameEngine::setSeed(int seed){
    this->seed = seed;
}

void GameEngine::setPlayerStartingNextRound(int playerNo){
    this->playerStartingNextRound = playerNo;
}

void GameEngine::setCurrentTurn(int playerNo){
    this->currentTurn = playerNo;
}

void GameEngine::swapCurrentTurn(){
    this->currentTurn++;
    if(this->currentTurn == noOfPlayers){
        this->currentTurn = 0;
    }
}


//getters
Player* GameEngine::getPlayerOne() const{
    //return playerOne;
    return nullptr;
}

Player* GameEngine::getPlayerTwo() const{
    //return playerTwo;
    return nullptr;
}

std::shared_ptr<Factory> GameEngine::getFactory(unsigned const int number) const{
    return factories[number];
}

LinkedList* GameEngine::getTileBag() const{
    return bag;
}

LinkedList* GameEngine::getBoxLid() const{
    return boxLid;
}

int GameEngine::getPlayerStartingNextRound() const{
    return this->playerStartingNextRound;
}

std::shared_ptr<Player> GameEngine::getCurrentPlayer() const{
    // Player* toReturn;
    // if(this->playerOne->getName() == this->currentTurn){
    //     toReturn = playerOne;
    // }else{
    //     toReturn = playerTwo;
    // }

    return this->players[this->currentTurn];
}

int GameEngine::getCurrentTurn() const{
    return this->currentTurn;
}

int GameEngine::getSeed() const{
    return this->seed;
}


//gameplay

void GameEngine::newGame(const std::string playerNames[], 
    int noOfPlayers, int noOfFactories, std::string gameMode) {

    this->noOfPlayers = noOfPlayers;
    this->noOfCentralFactories = noOfFactories;
    if(noOfFactories == 2){
        this->use2ndFactory = true;
    }

    determineGameMode(gameMode);
    instantiateFactories();

    for(int i = 0; i < noOfPlayers; ++i){
        std::shared_ptr<Player> player = std::make_shared<Player>(playerNames[i], i, gameMode);
        player->setPoints(0);
        player->getMosaicStorage()->getMosaic()->resetPoints();
        this->players.push_back(player);
    }

    currentTurn = 0;
    playerStartingNextRound = 1;

    populateBagAndShuffle();
    populateFactories();
}

void GameEngine::determineGameMode(std::string gameMode){
    if(gameMode == "grey"){
        this->greyMode = true;
        this->sixBySixMode = false;
        this->standardMode = false;
    } else if(gameMode == "six"){
        this->greyMode = false;
        this->sixBySixMode = true;
        this->standardMode = false;
    }
}

int GameEngine::playerTurn(std::string playerTurnCommand){
    Input input;
    int toReturn = 1;
    std::stringstream commandLine(playerTurnCommand);
    std::string commandPart;

    //0 = "turn", 1 = factory num, 2 = Tile, 3 = storage row, 4 = centralFactoryNo
    std::string commands[5];

    int counter = 0;
    while (commandLine.good()) {
        commandLine >> commandPart;
        commands[counter] = commandPart;
        counter++;
    }

    if(use2ndFactory && !normalFactoriesAreEmpty() && (commands[1] != "0" && commands[1] != "1")){
        gec->promptUser("Which central factory do you want to discard to?");
        commands[4] = input.getString();
    }else{
        commands[4] = commands[1];
    }
    
    if (!this->use2ndFactory){
        commands[4] = "0";
    }

    for(int i = 0; i<5; ++i){
        std::cout <<"Printing command [" << i << "] : " + commands[i] <<std::endl;
    }

    if(commands[0] == "turn"){

        //pass by ref to get converted values (if possible)
        int factoryNo;
        Type tileType;
        int storageRow;
        int centralFactoryNo;

        //validate the three command arguments first before proceeding
        if  (checkCommand1(commands[1], factoryNo) 
            && checkCommand2(commands[2], tileType) 
            && checkCommand3(commands[3],storageRow) 
            && checkCommand4(commands[4],centralFactoryNo,factoryNo)){
                std::cout<<">>>>>>>>>>>>TILE TYPE"<< tileType <<std::endl;
            if(factoryNo == 0 || (use2ndFactory && (factoryNo == 0 || factoryNo == 1))){
                if(centralFactoryOnlyHasFirstTile(centralFactoryNo)){
                    toReturn = Error_Message::NO_TILES_IN_CENTRAL;
                }
            }

            if(!tileExistsInFactory(tileType, factoryNo)){
                toReturn = Error_Message::COLOUR_NOT_IN_FACTORY;
            }

            //continue if the aforementioned checks pass
            if(toReturn == Error_Message::SUCCESS){

                std::cout<<"Homie g bro 2"<<std::endl;
                if (commands[3] != "B") {
                    if (moveTilesFromFactory(this->getCurrentPlayer(),factoryNo,
                        centralFactoryNo,(storageRow-1),tileType, false)) {
                        toReturn = Error_Message::SUCCESS;
                    } else {
                    std::cout<<"Homie g bro NOOOOOOOOOO"<<std::endl;
                        toReturn = Error_Message::INVALID_MOVE;
                    }
                } else {
                    if (moveTilesFromFactory(this->getCurrentPlayer(),factoryNo,
                        centralFactoryNo,(storageRow-1),tileType, true)) {
                        toReturn = Error_Message::SUCCESS;
                    } else {
                        toReturn = Error_Message::INVALID_MOVE;
                    }
                }
            }  
        } else{
            toReturn = Error_Message::INVALID_COMMAND;
        }
    } else if (commands[0] == "save") {
        GameEngineIO* geIO = new GameEngineIO(this);
        geIO->saveGame(commands[1]);
        toReturn = Error_Message::SAVED;
        delete geIO;
    } else {
        toReturn = Error_Message::INVALID_COMMAND;
    }

    return toReturn;
}

//check if input is int and valid
bool GameEngine::checkCommand1(const std::string input, int& factoryNo){
    bool success = inputIsInt(input);
    int maxFactoryNo = 5;
    if(use2ndFactory){
        maxFactoryNo = 6;
    }

    if(success){
        std::stringstream factoryNoAsString(input);
        factoryNoAsString >> factoryNo;       
        if(factoryNo < 0 || factoryNo > maxFactoryNo){
            success = false;
        }
    }
    return success;
}

//check if input isn't first player tile and is a valid colour
bool GameEngine::checkCommand2(const std::string input, Type& tileType){
    bool success = true;
    if (input.length() != 1) {
        success = false;
    } else if (input[0] == 'F') {
        success = false;
    } else {
        success = changeType(tileType,input[0]);
    }
    return success;
}

//check if storage row is convertible to int or if trying to move to broken tiles
bool GameEngine::checkCommand3(const std::string input, int& storageRow){
    bool success = false;
    int storageRowMax = 5;

    if(sixBySixMode)
        storageRowMax = 6;

    if(input.length() == 1){
        success = true;
        if (inputIsInt(input)) {
            std::stringstream storageRowNoAsString(input);
            storageRowNoAsString >> storageRow;

            if(storageRow < 1 || storageRow > storageRowMax)
                success = false;

        } else if (input[0] != 'B') {
            success = false;
        }
    }
    return success;
}

// check if user has specified a correct central factory no
bool GameEngine::checkCommand4(const std::string input, int& centralFactoryNo, int factoryNo){
    bool success = false;
    if(normalFactoriesAreEmpty()){
        centralFactoryNo = factoryNo;
        success = true;
    }

    else if(input.length() == 1){
        success = true;
        if (inputIsInt(input)) {
            std::stringstream centralFactoryNoAsString(input);
            centralFactoryNoAsString >> centralFactoryNo;

            if(centralFactoryNo < 0 || centralFactoryNo > 1)
                success = false;

        } 
    }
    return success;
}

bool GameEngine::inputIsInt(const std::string input){
    bool success = true;
    for(unsigned int i = 0; i < input.length(); i++){
        if(std::isalpha(input[i]))
            success = false;
    }
    return success;
}

bool GameEngine::normalFactoriesAreEmpty(){
    bool empty = true;
    bool breakLoop = false;
    int counter = 1;

    if(use2ndFactory)
        counter = 2;

    int stopIndex = counter + NUM_NORMAL_FACTORIES;
    
    while( counter < stopIndex && breakLoop == false){
        if(factories[counter]->getAllTiles().size() > 0){
            empty = false;
            breakLoop = true;
        }
        counter++;
    }

    return empty;
}

bool GameEngine::tileExistsInFactory(Type tileType, int factoryNo){
    bool success = false;
    std::vector<std::shared_ptr<Tile>> chosenFactory = factories[factoryNo]->getAllTiles();
    int size = chosenFactory.size();

    for(int i = 0; i < size; ++i){
        if(chosenFactory[i]->getType() == tileType)
            success = true;
    }
    return success;
}

bool GameEngine::centralFactoryOnlyHasFirstTile(int centralFactoryNo){
    bool success = false;
    std::vector<std::shared_ptr<Tile>> centralFactory = factories[centralFactoryNo]->getAllTiles();
    int size = centralFactory.size();

    if(size == 1 && centralFactory[0]->getType() == Type::FIRST_PLAYER)
        success = true;

    return success;
}

//must check each round if there is a full row so the game can end
bool GameEngine::winConditionMet(){
    bool winConditionMet = false;

    for(int i = 0; i<noOfPlayers; ++i){
        if(this->players[i]->getMosaicStorage()->getMosaic()->findFullRow()){
            winConditionMet = true;
        }
    }

    return winConditionMet;
}

//convert character version of tiles to enums for more efficient use in the program
bool GameEngine::changeType(Type& tileType, char tileChar){
    bool changed = true;
    if (tileChar == RED_CHAR) {
        tileType = Type::RED;
    } else if (tileChar == YELLOW_CHAR) {
        tileType = Type::YELLOW;
    } else if (tileChar == DARKBLUE_CHAR) {
        tileType = Type::DARK_BLUE;
    } else if (tileChar == LIGHTBLUE_CHAR) {
        tileType = Type::LIGHT_BLUE;
    } else if (tileChar == BLACK_CHAR) {
        tileType = Type::BLACK;
    } else if (tileChar == ORANGE_CHAR) {
        tileType = Type::ORANGE;
    } else if (tileChar == FIRST_CHAR) {
        tileType = Type::FIRST_PLAYER;
    } else {
        changed = false;
    }
    return changed;
}

//calculates the points gained and lost through broken tiles and mosaic 
void GameEngine::calculatePointsPerRound() {

    for(int i = 0; i<noOfPlayers; ++i){
        std::shared_ptr<MosaicStorage> playerMosaicStorage = this->players[i]->getMosaicStorage();
        int ptsLost = playerMosaicStorage->getBrokenTiles()->calculatePointsLost();
        int ptsWon = playerMosaicStorage->getMosaic()->getPointsThisRound();
        int endOfRoundPts = (this->players[i]->getPoints() + ptsWon) - ptsLost;
        std::cout << "CURRENT PLAYERRRRRR : " << this->players[i]->getName() << std::endl;
        std::cout<<"CURRENT POINTSSSSSSS : " << this->players[i]->getPoints() << std::endl;
        std::cout<<"POINTS WONNNNNNNNNNNN : " << ptsWon << std::endl;
        std::cout<<"POINTS LOSSSSSSSSSSSSS : " << ptsLost << std::endl;
        std::cout<<"BROKEN TILE SIZZEEEEEEEEEEE: " << playerMosaicStorage->getBrokenTiles()->getSize() << std::endl;
        if (endOfRoundPts < 0)
            endOfRoundPts = 0;
        this->players[i]->setPoints(endOfRoundPts);
        
        playerMosaicStorage->getMosaic()->resetPoints();
        playerMosaicStorage->endOfRoundDiscardBrokenTiles();
    }

    // std::shared_ptr<MosaicStorage> playerOneMosaicStorage = playerOne->getMosaicStorage();
    // std::shared_ptr<MosaicStorage> playerTwoMosaicStorage = playerTwo->getMosaicStorage();

    // int playerOneLostPoints = playerOneMosaicStorage->getBrokenTiles()->calculatePointsLost();
    // int playerTwoLostPoints = playerTwoMosaicStorage->getBrokenTiles()->calculatePointsLost();

    // int playerOnePointsThisRound = playerOneMosaicStorage->getMosaic()->getPointsThisRound();
    // int playerTwoPointsThisRound = playerTwoMosaicStorage->getMosaic()->getPointsThisRound();

    // int playerOneEndRoundPoints = (playerOne->getPoints() + playerOnePointsThisRound)-playerOneLostPoints;
    // int playerTwoEndRoundPoints = (playerTwo->getPoints() + playerTwoPointsThisRound)-playerTwoLostPoints;

    // if (playerTwoEndRoundPoints < 0)
    //     playerTwoEndRoundPoints = 0;
    // if (playerOneEndRoundPoints < 0)
    //     playerOneEndRoundPoints = 0;

    // playerOne->setPoints(playerOneEndRoundPoints);
    // playerTwo->setPoints(playerTwoEndRoundPoints);

    // playerOneMosaicStorage->getMosaic()->resetPoints();
    // playerTwoMosaicStorage->getMosaic()->resetPoints();
    // playerOneMosaicStorage->endOfRoundDiscardBrokenTiles();
    // playerTwoMosaicStorage->endOfRoundDiscardBrokenTiles();

    gec->playerEndOfRoundResult(this->players, this->noOfPlayers);

}

//terminates the round when there are no more tiles in the factories
bool GameEngine::endOfRoundConditionMet(){
    bool endOfRound = false;
    int counter = 0;
    for(int i = 0; i < (noOfCentralFactories + NUM_NORMAL_FACTORIES); ++i){
        if(factories[i]->getAllTiles().size() == 0)
            ++counter;
    }
    if(counter == (noOfCentralFactories + NUM_NORMAL_FACTORIES))
        endOfRound = true;

    return endOfRound;
}

void GameEngine::movePlayerTilesToMosaic(){
    if(greyMode){

        Input input;
        gec->playerBoardUpdate(players, noOfPlayers, sixBySixMode);

        for(int i = 0; i<noOfPlayers; ++i){
            std::shared_ptr<MosaicStorage> playerMosaicStorage = this->players[i]->getMosaicStorage();
            for(int row = 0; row<NUM_NORMAL_FACTORIES; ++row){
                if(playerMosaicStorage->isRowFull(row)){
                    bool success = false;
                    int col = 0;
                    while(success == false){
                        gec->promptUser(this->players[i]->getName() + "'s turn to move from Storage to Mosaic.");
                        gec->promptUser("Where do you want to move row " 
                                + std::to_string(row+1) 
                                + " tiles to? (Enter the column index)");
                        std::string colAsString = input.getString();
                        if (input.inputIsInt(colAsString)) {
                            std::stringstream sstream (colAsString);
                            sstream >> col;
                            if(col >= 1 && col <= 5){
                                success = playerMosaicStorage->greyModeEndOfRoundMove(row,col-1);
                            }
                        }
                        gec->playerBoardUpdate(this->players, noOfPlayers,sixBySixMode);
                        if(!success){
                            std::string output = "Error: Please enter a number from 1-5.\n";
                            output+= "Remember: Columns and Rows can only have 1 of a color.\n";
                            gec->promptUser(output);;
                        }
                    }
                }
            }
        
        }
    }
    else{
        for(int i = 0; i<noOfPlayers; ++i){
            std::shared_ptr<MosaicStorage> playerMosaicStorage = this->players[i]->getMosaicStorage();
            playerMosaicStorage->endOfRoundMove(i);
        }
    }

    // std::shared_ptr<MosaicStorage> playerOneMosaicStorage = playerOne->getMosaicStorage();
    // std::shared_ptr<MosaicStorage> playerTwoMosaicStorage = playerTwo->getMosaicStorage();

    // playerOneMosaicStorage->endOfRoundMove();
    // playerTwoMosaicStorage->endOfRoundMove();
}

void GameEngine::endOfRoundPreparations(){
    movePlayerTilesToMosaic();
    calculatePointsPerRound();
    for(int i = 0; i<noOfPlayers; ++i){
        moveTilesToLid(this->players[i]);
    }
    // moveTilesToLid(playerOne);
    // moveTilesToLid(playerTwo);
}

bool GameEngine::moveTilesFromFactory(std::shared_ptr<Player> player,unsigned const int factoryNumber,int centralFactoryNo,unsigned const int row, const Type type, const bool toBroken) {
    bool turnSuccess = true;

    if (toBroken)
        //need to take into consideration wanting to move tiles to broken tiles manually
        moveTilesToBrokenTiles(player, factoryNumber, type);
        
    else if (player->getMosaicStorage()->isValidAdd(type, row))
        //player has chosen to put the tiles from the factory somewhere in their mosaic storage
        moveTilesToMosaicStorage(player, factoryNumber, centralFactoryNo, row, type);
        
    else
        //no turns have been taken 
        turnSuccess = false;
    return turnSuccess;
}

void GameEngine::moveTilesToMosaicStorage(std::shared_ptr<Player> player, 
    unsigned const int factoryNumber,int centralFactoryNo, unsigned const int row,const Type type){

    std::vector<std::shared_ptr<Tile>> allTiles =  factories[factoryNumber]->getCopiedTilesAndRemove();
    int size = allTiles.size();
    for(int i = 0; i < size; i++){
        std::shared_ptr<Tile> tileToAdd = allTiles[i];
        if(tileToAdd->getType() == Type::FIRST_PLAYER){
            //automatically move the first player tile to the broken tiles
            player->getMosaicStorage()->getBrokenTiles()->addTile(tileToAdd);
            this->setPlayerStartingNextRound(player->getPlayerNo());

            if(use2ndFactory)
                removeOtherFirstPlayerToken(centralFactoryNo);

        } else if(allTiles[i]->getType() == type){
            player->getMosaicStorage()->addTile(tileToAdd, row);
        } else{
            //add the remaining unchosen tiles to central factory
            factories[centralFactoryNo]->addTile(tileToAdd);
        }  
    }
}

void GameEngine::removeOtherFirstPlayerToken(int centralFactory){
    std::shared_ptr<Factory> factory = nullptr;
    if( centralFactory == 0)
        factory = factories[1];
    else
        factory = factories[0];

    std::vector<std::shared_ptr<Tile>> allTiles = factory->getCopiedTilesAndRemove();
    int size = allTiles.size();
    for(int i = 0; i < size; i++){
        std::shared_ptr<Tile> tileToAdd = allTiles[i];
        if(tileToAdd->getType() != Type::FIRST_PLAYER)
            factory->addTile(tileToAdd);
    }

}

void GameEngine::moveTilesToBrokenTiles(std::shared_ptr<Player> player, 
    unsigned const int factoryNumber, const Type type){

    int maxBrokenTiles = 7;
    if(sixBySixMode)
        maxBrokenTiles=8;
    std::vector<std::shared_ptr<Tile>> allTiles =  factories[factoryNumber]->getCopiedTilesAndRemove();
    std::shared_ptr<MosaicStorage> mosaicStorage = player->getMosaicStorage();
    BrokenTiles* brokenTiles = mosaicStorage->getBrokenTiles();
    
        int size = allTiles.size();
        for (int i = 0; i < size; ++i) {
            std::shared_ptr<Tile> tileToAdd = allTiles[i];
            if (allTiles[i]->getType() == type) {
                //make sure that the player can only have a max of 7 tiles; the rest go to the box lid if the max is reached
                if(brokenTiles->getSize() < maxBrokenTiles)
                    brokenTiles->addTile(tileToAdd);
                else 
                    mosaicStorage->addTileToDiscardedTiles(tileToAdd);
            } else {
                factories[0]->addTile(tileToAdd);
            }
        }
}

//called at the end of each round to get rid of no longer usable tiles
void GameEngine::moveTilesToLid(std::shared_ptr<Player> player){
    std::vector<std::shared_ptr<Tile>> discarded = *(player->getMosaicStorage()->getDiscardedTiles());

    int discardedSize = discarded.size();

    for(int i = 0; i < discardedSize; i++){
        boxLid->addTileToFront(discarded[i]);
        discarded[i] = nullptr;
    }

    player->getMosaicStorage()->resetDiscardTilesVector();
} 

void GameEngine::refillBag() {
    int totalTiles = boxLid->getSize();

    for(int i = 0; i < totalTiles; ++i)
        bag->addTileToFront(boxLid->getAndRemoveFirstTile());
}

void GameEngine::populateFactories(){
    int startIndex = 1;
    int noOfTilesInFactory = 4;
    factories[0]->addTile(std::make_shared<Tile>(Type::FIRST_PLAYER));

    if(use2ndFactory){
        factories[1]->addTile(std::make_shared<Tile>(Type::FIRST_PLAYER));
        startIndex = 2;
    }
    if(sixBySixMode)
        noOfTilesInFactory = 5;

    //start at 1 so we don't populate the central factory
    for(int i = startIndex; i < (startIndex+5); i++){
        //fill each factory with noOfTilesInFactory tiles
        for(int j = 0; j < noOfTilesInFactory; ++j){
            if (bag->getSize() > 0) {
                factories[i]->addTile(bag->getAndRemoveFirstTile());
            } else {
                refillBag();
                factories[i]->addTile(bag->getAndRemoveFirstTile());
            }
        }
    }
}

void GameEngine::populateBagAndShuffle(){
    //populate array for later shuffling
    int bagToShuffleSize = 100;
    if(this->sixBySixMode){
        bagToShuffleSize = 120;
    }
    std::shared_ptr<Tile> bagToShuffle[bagToShuffleSize];
    int bagCount = 0;

    GameEngine::addTilesByColourToBag(Type::BLACK, bagToShuffle, bagCount);
    GameEngine::addTilesByColourToBag(Type::DARK_BLUE, bagToShuffle, bagCount);
    GameEngine::addTilesByColourToBag(Type::LIGHT_BLUE, bagToShuffle, bagCount);
    GameEngine::addTilesByColourToBag(Type::RED, bagToShuffle, bagCount);
    GameEngine::addTilesByColourToBag(Type::YELLOW, bagToShuffle, bagCount);
    if(this->sixBySixMode){
        GameEngine::addTilesByColourToBag(Type::ORANGE, bagToShuffle, bagCount);
    }
    GameEngine::shuffle(bagToShuffle, bagToShuffleSize);
    
    //add to linked list format (which is the one used for the rest of the game)
    for(int i = 0; i < bagToShuffleSize; i++){
        bag->addTileToFront(bagToShuffle[i]);
        bagToShuffle[i] = nullptr;
    }
}

//shuffle the bag through a Knuths/Fisher-Yates shuffle algorithm. Can't use seed 0.
void GameEngine::shuffle(std::shared_ptr<Tile> bagToShuffle[] , unsigned const int size){
    std::default_random_engine engine;

    //instantiate the engine with either the given seed or random seed
    if (seed != 0) {
        engine.seed(seed);
    } else {
        seed = std::random_device{}();
        engine.seed(seed);
    }

    for(int i = size - 1; i > 0 ; --i){
        //pick an index so that 0 <= j <= i
        std::uniform_int_distribution<int> uniform_dist(0, i);
        int j = uniform_dist(engine);

        //swap the tiles
        std::shared_ptr<Tile> temp = bagToShuffle[i];
        bagToShuffle[i] = bagToShuffle[j];
        bagToShuffle[j] = temp;
    }
}

//enables 20 tiles of each colour to be added to the tile bag for later shuffling
void GameEngine::addTilesByColourToBag(Type type, std::shared_ptr<Tile> bagToShuffle[], int &bagCount){
    int valToAssignBagCount = 0;
    int maxTiles = 20;

    for(int i = bagCount; i < (bagCount + maxTiles); ++i){
        valToAssignBagCount = i;
        bagToShuffle[i] = std::make_shared<Tile>(type);
    }

    bagCount = ++valToAssignBagCount;
}

/*
*Interpret the following integers as:
* 0: invalid command
* 1: successful turn
* 2: saved game
* 3: error due to nothing in central factory
* 4: colour not in factory
* 5: invalid moves
*/
std::string GameEngine::interpretPlayerTurn(const int result){
    std::string toReturn;
    if(result == Error_Message::INVALID_COMMAND)
        toReturn = "Error: Invalid Command.\n";
    if(result == Error_Message::SUCCESS)
        toReturn = "Turn Successful.\n";
    if(result == Error_Message::SAVED)
        toReturn = "Saved Game.\n";
    if(result == Error_Message::NO_TILES_IN_CENTRAL)
        toReturn = "Error: There is no tiles in the central factory!\n";
    if(result == Error_Message::COLOUR_NOT_IN_FACTORY)
        toReturn = "Error: The colour specified is not in this factory.\n";
    if(result == Error_Message::INVALID_MOVE)
        toReturn = "Error: The move you are trying to make is invalid.\n";

    return toReturn;
}

//loop enables the game to keep playing until someone wins or someone quits
void GameEngine::gameplayLoop(bool& endOfCommands, bool& continueMenuLoop) {
    Input input;
    while(!endOfCommands && !std::cin.eof() && !winConditionMet()){
        while(!endOfCommands && !endOfRoundConditionMet()){

            //output relevant information to players
            gec->boardComponentUpdate(factories, use2ndFactory);
            // gec->playerBoardUpdate(playerOne);
            // gec->playerBoardUpdate(playerTwo);
            //for(int i = 0; i<noOfPlayers; ++i){
                gec->playerBoardUpdate(this->players, noOfPlayers, sixBySixMode);
            //}
            gec->playerTurnUpdate(this->players[currentTurn]->getName());

            std::string playerCommand = "";
            std::string centralCommandToDiscardTo = "";
            int turnResult = 0;
            while(!endOfCommands && !std::cin.eof() && (turnResult != 1)){
                playerCommand = input.getString();
                turnResult = playerTurn(playerCommand);
                gec->playerTurnResult(interpretPlayerTurn(turnResult));
            }
            // This only runs for io redirection; program automatically exits if the eof is reached
            if(std::cin.eof()){
                gec->playerTurnResult("Program will now exit.");
                endOfCommands = true;
                continueMenuLoop = false;
            }

            swapCurrentTurn();
        }
        if(!endOfCommands){
            endOfRoundPreparations();
            populateFactories();
            setCurrentTurn(getPlayerStartingNextRound());
            int playerStartingNextRound = getPlayerStartingNextRound();
            playerStartingNextRound++;
            if( playerStartingNextRound == noOfPlayers){
                playerStartingNextRound = 0;
            }
            setPlayerStartingNextRound(playerStartingNextRound);
        }
    }

    //loop breaks so we can finalise scores and decide on winner
    if (winConditionMet()) {
        // gec->playerBoardUpdate(playerOne);
        // gec->playerBoardUpdate(playerTwo);
        //for(int i = 0; i<noOfPlayers; ++i){
                gec->playerBoardUpdate(this->players, noOfPlayers,sixBySixMode);
        //    }
        calculateEndGamePoints();

        // When testing, we save the game before it ends to see the end of game save file
        // if (testing) {
        //     GameEngineIO* geIO = new GameEngineIO(this);
        //     geIO->saveGame("actualoutcome.save");
        //     delete geIO;
        // }

        resetGame();
    }

}

void GameEngine::calculateEndGamePoints() {

    for(int i = 0; i<noOfPlayers; ++i){
        int additionalPts = this->players[i]->getMosaicStorage()->getMosaic()->calculateEndGamePoints();
        this->players[i]->setPoints(players[i]->getPoints() + additionalPts);
    }
    // int playerOneAdditionalPoints = playerOne->getMosaicStorage()->getMosaic()->calculateEndGamePoints();
    // int playerTwoAdditionalPoints = playerTwo->getMosaicStorage()->getMosaic()->calculateEndGamePoints();

    // playerOne->setPoints(playerOne->getPoints() + playerOneAdditionalPoints);
    // playerTwo->setPoints(playerTwo->getPoints() + playerTwoAdditionalPoints);

    gec->playerEndOfGameResult(this->players, this->noOfPlayers);
}

void GameEngine::resetGame(){
    
    //don't delete components as they get instantiated with GE
    for(int i = 0; i < (noOfCentralFactories + NUM_NORMAL_FACTORIES); i++){
        factories[i]->clear();
    }
    players.clear();
    factories.clear();
    bag->clear();
    boxLid->clear();
}

