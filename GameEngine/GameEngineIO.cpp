#include "GameEngine.h"
#include "GameEngineIO.h"


GameEngineIO::GameEngineIO(GameEngine* engine){
    this->gameEngine = engine;
    this->greyMode = false;
    this->sixBySixMode = false;
    this->standard = false;
    this->readError = false;
    this->index = 0;
}

GameEngineIO::~GameEngineIO(){}

void GameEngineIO::loadGame(std::string fileName) {

    // Load in each line of the file as a unique game info string.
    std::ifstream ifs (fileName);
    std::string currentInfo;
    std::getline(ifs, currentInfo);

    if(!ifs.is_open())
        throw "File not found";

    unsigned int i = 0;

    while (ifs.good()) {
        gameInfo.push_back(currentInfo);
        std::getline(ifs, currentInfo);
        ++i;
    }

    ifs.close();

    if(gameInfo.size() == LEGACY_SAVE){
        setLegacySaveSettings();
    }
    else{
        // Load game settings
        determineGameMode(gameInfo[0]);
        determineNoOfPlayers(gameInfo[1]);
        determineNoOfCentralFactories(gameInfo[2]);
    }
    
    // Set game settings according to above
    gameEngine->loadGameSettings(this->noOfPlayers,this->noOfCentralFactories,gameInfo[0]);

    // load the individual components from the file
    loadPlayers();
    loadFactories();

    loadMosaics();
    loadStorageArea();
    loadBrokenTiles();

    loadLid();
    loadBag();
    loadSeed();
}

void GameEngineIO::setLegacySaveSettings(){
    this->legacySave = true;
    this->standard = true;
    this->greyMode = false;
    this->sixBySixMode = false;
    this->noOfTilesPerFactory = 4;
    this->noOfStorageRows = 5;
    this->noOfPlayers = 2;
    this->noOfFactories = 5;
    this->noOfCentralFactories = 1;
}

void GameEngineIO::determineNoOfCentralFactories(std::string noOfCentralFactories){
    std::stringstream noOfCentralFactoriesAsString(noOfCentralFactories);
    noOfCentralFactoriesAsString >> this->noOfCentralFactories;
}

void GameEngineIO::determineNoOfPlayers(std::string playerNo){
    std::stringstream noOfPlayers(playerNo);
    noOfPlayers >> this->noOfPlayers;

    if(this->noOfPlayers == 2)
        this->noOfFactories = 5;
    else if(this->noOfPlayers == 3)
        this->noOfFactories = 7;
    else if(this->noOfPlayers == 4)
        this->noOfFactories = 9;
}

void GameEngineIO::determineGameMode(std::string gameMode){
     //Determine game mode
    if(gameMode == "grey"){
        this->greyMode = true;
        this->sixBySixMode = false;
        this->standard = false;
        this->noOfTilesPerFactory = 4;
        this->noOfStorageRows = 5;
    }else if(gameMode == "six"){
        this->sixBySixMode = true;
        this->greyMode = true;
        this->standard = false;
        this->noOfTilesPerFactory = 5;
        this->noOfStorageRows = 6;
    }if(gameMode == "standard"){
        this->standard = true;
        this->greyMode = false;
        this->sixBySixMode = false;
        this->noOfTilesPerFactory = 4;
        this->noOfStorageRows = 5;
    }
}

void GameEngineIO::loadPlayers(){

    if (gameInfo[0] == gameInfo[1]) {
        throw "Both players cannot have the same name.";
    }

    if(legacySave)
        this->index = 0;
    else
        this->index = 3;

    for(unsigned int i = 0; i < noOfPlayers; ++i){
        this->gameEngine->setPlayer(gameInfo[index],i,gameInfo[0]);
        ++this->index;
    }

    // setting points
    for(unsigned int i = 0; i < noOfPlayers; ++i){
        std::stringstream playerPoints(gameInfo[index]);
        int points;
        playerPoints >> points;
        std::shared_ptr<Player> player = this->gameEngine->getPlayer(i);
        player->setPoints(points);
        ++this->index;
    }

    // Load Next Turn info into game
    std::stringstream currentTurnAsString(gameInfo[index]);
    if(legacySave){
        std::vector<std::shared_ptr<Player>> players = gameEngine->getPlayers();
        int size = players.size();
        for(int i = 0; i < size; ++i){
            if(players[i]->getName() == gameInfo[index])
                gameEngine->setCurrentTurn(i);
        }
    }else{
        int currentTurn;
        currentTurnAsString >> currentTurn;
        gameEngine->setCurrentTurn(currentTurn);
    }
    ++this->index;
}

void GameEngineIO::loadFactories(){
    for (unsigned int i = 0; i < (this->noOfFactories + this->noOfCentralFactories); ++i) {
        std::stringstream factoryStream(gameInfo[index]);
        unsigned int counter = 0;
        char toAdd;
        factoryStream >> toAdd;
        while (factoryStream.good()) {
            Type tileType = Type::NONE;
            if(toAdd == '.'){
                // Do nothing
            }
            // If the Type is changed from NONE to something else, then add it to the Factory.
            else if (gameEngine->changeType(tileType, toAdd)) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

                gameEngine->getFactory(i)->addTile(tile);
            } else {
                readError = true;
            }
            factoryStream >> toAdd;
            ++counter;
        }
        ++index;
    }
}

void GameEngineIO::loadMosaics(){
    // Load Player 1 and 2 Mosaics into game 

    for (unsigned int playerNum = 0; playerNum < this->noOfPlayers; ++playerNum) {

        for (unsigned int row = 0; row < noOfStorageRows; ++row) {

            std::stringstream playerMosaicStream(gameInfo[this->index]);
            char toAdd;
            playerMosaicStream >> toAdd;

            for (unsigned int col = 0; col < noOfStorageRows; ++col) {
                if (playerMosaicStream.good()) {
                    Type tileType = Type::NONE;
                    if (gameEngine->changeType(tileType, toAdd)) {
                        std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                        std::shared_ptr<Player> player = gameEngine->getPlayer(playerNum);
                        if (!player->getMosaicStorage()->getMosaic()->addTile(tile, row, col)) {
                            readError = true;
                            tile = nullptr;
                        }
                    }
                    playerMosaicStream >> toAdd;
                }
            }
            ++this->index;
        }
    }
    for(unsigned int i = 0; i <noOfPlayers; ++i){
        std::shared_ptr<Player> player = gameEngine->getPlayer(i);
        player->getMosaicStorage()->getMosaic()->resetPoints();
    }

}

void GameEngineIO::loadStorageArea(){
     // Load Player 1 and 2 MosaicStorages into game. 

    for (unsigned int playerNum = 0; playerNum < this->noOfPlayers; ++playerNum) {

        for (unsigned int row = 0; row < this->noOfStorageRows; ++row) {

            std::stringstream mosaicStorageStream(gameInfo[index]);

            char toAdd;
            mosaicStorageStream >> toAdd;

            while (mosaicStorageStream.good()) {
                Type tileType = Type::NONE;
                if (gameEngine->changeType(tileType, toAdd)) {
                    std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                    std::shared_ptr<Player> player = gameEngine->getPlayer(playerNum);

                    if (player->getMosaicStorage()->isValidAdd(tileType, row)) {
                        player->getMosaicStorage()->addTile(tile, row);
                    } else {
                        readError = true;
                        tile = nullptr;
                    }
                }
                mosaicStorageStream >> toAdd;
            }
            ++index;
        }
    }
}

void GameEngineIO::loadBrokenTiles(){
    // Load Player 1 and 2 Broken Tiles into game

    for (unsigned int playerNum = 0; playerNum < this->noOfPlayers; ++playerNum) {

        std::stringstream brokenTileStream(gameInfo[index]); // 31 and 32
        char toAdd;
        brokenTileStream >> toAdd;

        while (brokenTileStream.good()) {
            Type tileType = Type::NONE;
            if(toAdd == '.'){
                // Do nothing
            }
            else if (gameEngine->changeType(tileType, toAdd)) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                std::shared_ptr<Player> player = gameEngine->getPlayer(playerNum);
                if(tileType == Type::FIRST_PLAYER){
                    gameEngine->setPlayerStartingNextRound(playerNum);
                }
                player->getMosaicStorage()->getBrokenTiles()->addTile(tile);
            } else {
                readError = true;
            }
            brokenTileStream >> toAdd;
        }
        ++index;
    }

}

void GameEngineIO::loadLid(){
    
    // Load Box Lid Tiles into  game
    std::stringstream boxLidStream(gameInfo[index]);
    // Going to need to traverse the vector backwards since our box lid only has an add to front method
    std::vector<char> tilesToAdd;
    char toAdd;
    boxLidStream >> toAdd;

    while (boxLidStream.good()) {
        tilesToAdd.push_back(toAdd);
        boxLidStream >> toAdd;
    }

    unsigned int lastIndex = tilesToAdd.size() - 1;
    for (int i = lastIndex; i >= 0; --i) {

        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getBoxLid()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    tilesToAdd.clear();
    ++index;

}

void GameEngineIO::loadBag(){
    // Load Bag Tiles into game
    std::stringstream tileBagStream(gameInfo[index]);
    char toAdd;
    std::vector<char> tilesToAdd;
    // Going to need to traverse the vector backwards since our tile bag only has an add to front method
    tileBagStream >> toAdd;

    while (tileBagStream.good()) {
        tilesToAdd.push_back(toAdd);
        tileBagStream >> toAdd;
    }

    unsigned int lastIndex = tilesToAdd.size() - 1;
    for (int i = lastIndex; i >= 0; --i) {
        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getTileBag()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    tilesToAdd.clear();
    ++index;
}

void GameEngineIO::loadSeed(){
    int seed;
    std::stringstream seedStream(gameInfo[index]);
    seedStream >> seed;
    gameEngine->setSeed(seed);
}

void GameEngineIO::saveGame(std::string fileName, std::string gameMode, int noOfPlayers) {
    std::ofstream outFile;
    outFile.open(fileName);
    
    determineGameMode(gameMode);

    std::vector<std::shared_ptr<Player>> players = gameEngine->getPlayers();

    if(outFile.good()){

        outFile << gameEngine->getGameModeAsString() << std::endl;
        outFile << gameEngine->getPlayers().size() << std::endl;
        outFile << gameEngine->getNoOfCentralFactories() << std::endl;

        //save player info
        for(unsigned int i = 0; i < players.size(); ++i){
            outFile << players[i]->getName() <<std::endl;
        } 

        for(unsigned int i = 0; i < players.size(); ++i){
            outFile << players[i]->getPoints() <<std::endl;
        }

        outFile << gameEngine->getCurrentTurn() << std::endl; 

        //save factories
        int noOfTotalFactories = 
            gameEngine->getNoOfCentralFactories() + gameEngine->getNoOfNormalFactories();

        for(int i = 0; i < noOfTotalFactories; i++){
            outFile << gameEngine->getFactory(i)->toSave() << std::endl;
        }

        //save mosaics 
        int noOfMosaicRows = 5;
        if(this->sixBySixMode){
            noOfMosaicRows = 6;
        }

        for(unsigned int i = 0; i < players.size(); ++i){
            for(int j = 0; j < noOfMosaicRows; ++j){
                outFile << players[i]->getMosaicStorage()->getMosaic()->rowToSave(j) << std::endl;
            }
        }

        //save storage areas
        for(unsigned int i = 0; i < players.size(); ++i){
            for(int j = 0; j < noOfMosaicRows; ++j){
                outFile << players[i]->getMosaicStorage()->rowToSave(j) << std::endl;
            }
        }

        //save brokenTiles
        for(unsigned int i = 0; i < players.size(); ++i){
            outFile << players[i]->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
        }

        //save box lid tiles
        outFile << gameEngine->getBoxLid()->toSave() <<std::endl;

        //save bag tiles
        outFile << gameEngine->getTileBag()->toSave() << std::endl;

        //save random seed
        outFile << gameEngine->getSeed() << std::endl;
    }
    outFile.close();
}