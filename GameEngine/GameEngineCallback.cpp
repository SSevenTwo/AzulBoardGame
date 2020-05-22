#include "GameEngineCallback.h"

GameEngineCallback::GameEngineCallback() {

}

GameEngineCallback::~GameEngineCallback() {

}

void GameEngineCallback::playerBoardUpdate(std::vector<std::shared_ptr<Player>> players, int noOfPlayers) {
    std::string outputString;

    std::shared_ptr<MosaicStorage> mStorage = players[0]->getMosaicStorage();
    Mosaic* mosaic = players[0]->getMosaicStorage()->getMosaic();    
    std::shared_ptr<MosaicStorage> mStorage1 = players[1]->getMosaicStorage();
    Mosaic* mosaic1 = players[1]->getMosaicStorage()->getMosaic();

    outputString = "Mosaic for " + players[0]->getName() + "\t\t\t" + "Mosaic for " + players[1]->getName() + "\n"
        + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t" + mStorage1->rowToString(0) + mosaic1->rowToString(0) + "\n"
        + mStorage->rowToString(1) + mosaic->rowToString(1) + "\t" + mStorage1->rowToString(1) + mosaic1->rowToString(1) + "\n"
        + mStorage->rowToString(2) + mosaic->rowToString(2) + "\t" + mStorage1->rowToString(2) + mosaic1->rowToString(2) + "\n"
        + mStorage->rowToString(3) + mosaic->rowToString(3) + "\t" + mStorage1->rowToString(3) + mosaic1->rowToString(3) + "\n"
        + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t" + mStorage1->rowToString(4) + mosaic1->rowToString(4) + "\n"
        + players[0]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t" + players[1]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";

    if( noOfPlayers == 3){
        mStorage = players[2]->getMosaicStorage();
        mosaic = players[2]->getMosaicStorage()->getMosaic();    
        outputString += "Mosaic for " + players[2]->getName() + "\n"
        + mStorage->rowToString(0) + mosaic->rowToString(0) + "\n"
        + mStorage->rowToString(1) + mosaic->rowToString(1) + "\n"
        + mStorage->rowToString(2) + mosaic->rowToString(2) + "\n"
        + mStorage->rowToString(3) + mosaic->rowToString(3) + "\n"
        + mStorage->rowToString(4) + mosaic->rowToString(4) + "\n"
        + players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\n";
    }

    else if( noOfPlayers == 4){
        mStorage = players[2]->getMosaicStorage();
        mosaic = players[2]->getMosaicStorage()->getMosaic();   
        mStorage = players[3]->getMosaicStorage();
        mosaic = players[3]->getMosaicStorage()->getMosaic();    
        outputString += "Mosaic for " + players[2]->getName() + "\t\t\t" + "Mosaic for " + players[3]->getName() + "\n"
        + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t" + mStorage1->rowToString(0) + mosaic1->rowToString(0) + "\n"
        + mStorage->rowToString(1) + mosaic->rowToString(1) + "\t" + mStorage1->rowToString(1) + mosaic1->rowToString(1) + "\n"
        + mStorage->rowToString(2) + mosaic->rowToString(2) + "\t" + mStorage1->rowToString(2) + mosaic1->rowToString(2) + "\n"
        + mStorage->rowToString(3) + mosaic->rowToString(3) + "\t" + mStorage1->rowToString(3) + mosaic1->rowToString(3) + "\n"
        + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t" + mStorage1->rowToString(4) + mosaic1->rowToString(4) + "\n"
        + players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t" + players[3]->getMosaicStorage()->getBrokenTiles()->toString() + "\n";
    }
    std::cout << outputString << std::endl;
    // mosaic->templateRowToString(0)
}

void GameEngineCallback::playerTurnUpdate(std::string playerName) {
    std::cout<< "It is now " << playerName <<"'s turn. " << std::endl;
}

void GameEngineCallback::playerTurnResult(std::string turnResult) {
    std::cout << turnResult << std::endl;
}

void GameEngineCallback::playerEndOfRoundResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers) {
    
    for(int i = 0; i<noOfPlayers; ++i){
        std::cout << players[i]->getName() << " Points: " << players[i]->getPoints() << std::endl;
    }
    
    std::cout<< "\n=====End of round - Beginning next round!=====\n" << std::endl;
    // std::cout << players[0]->getName() << " Points: " << players[0]->getPoints() << std::endl;
    // std::cout << players[1]->getName() << " Points: " << players[1]->getPoints() << std::endl;
    // std::cout<< "\n=====End of round - Beginning next round!=====\n" << std::endl;
}

void GameEngineCallback::playerEndOfGameResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers) {
    for(int i = 0; i<noOfPlayers; ++i){
        std::cout << players[i]->getName() << " Points: " << players[i]->getPoints() << std::endl;
    }
    std::cout<< "\n=================End of Game=================" << std::endl;
    int winner = 0;
    int pointsOfWinner = players[0]->getPoints();
    for(int i = 0; i<noOfPlayers; ++i){
        if(players[i]->getPoints() > pointsOfWinner){
            winner = i;
            pointsOfWinner = players[i]->getPoints();
        }
    }

    std::cout << "Winner is " << players[winner]->getName() << "!\n" << std::endl;
    // std::cout << players[0]->getName() << " Points: " << players[0]->getPoints() << std::endl;
    // std::cout << players[1]->getName() << " Points: " << players[1]->getPoints() << std::endl;
    // std::cout<< "\n=================End of Game=================" << std::endl;
    // if (players[0]->getPoints() > players[1]->getPoints()) {
    //     std::cout << "Winner is " << players[0]->getName() << "!\n" << std::endl;
    // } else {
    //     std::cout << "Winner is " << players[1]->getName() << "!\n" << std::endl;
    // }
}

void GameEngineCallback::boardComponentUpdate(std::vector<std::shared_ptr<Factory>> factories, bool use2ndFactory) {
    std::cout<< "--------------------Next Turn------------------\n" << std::endl;
    std::string outputString = "";
    int noOfCenterFactories = 1;
    if(use2ndFactory)
        noOfCenterFactories = 2;

    for(int i = 0; i < noOfCenterFactories; ++i){

        std::vector<std::shared_ptr<Tile>> tilesOfCenter = factories[i]->getAllTiles();
        outputString += "Factory " + std::to_string(i) + " (Center): ";
        int size = tilesOfCenter.size();
        for(int j = 0; j < size; ++j){
            outputString.push_back(tilesOfCenter[j]->getColourType());
            outputString += " ";
        }
        outputString += "\n";
    }

    for(unsigned int i = (noOfCenterFactories); i < factories.size(); i++){
        std::vector<std::shared_ptr<Tile>> tiles = factories[i]->getAllTiles();
        outputString += "Factory " + std::to_string(i) += ": ";
        int size = tiles.size();
        for(int j = 0; j < size; ++j){
            outputString.push_back(tiles[j]->getColourType());
            outputString += " ";
        }
        outputString += "\n";
    }

    std::cout << outputString << std::endl;
}