#include "GameEngineCallback.h"

GameEngineCallback::GameEngineCallback() {

}

GameEngineCallback::~GameEngineCallback() {

}

void GameEngineCallback::playerBoardUpdate(std::vector<std::shared_ptr<Player>> players, int noOfPlayers, bool sixBySix) {
    std::string outputString;

    std::shared_ptr<MosaicStorage> mStorage = players[0]->getMosaicStorage();
    std::shared_ptr<Mosaic> mosaic = players[0]->getMosaicStorage()->getMosaic();    
    std::shared_ptr<MosaicStorage> mStorage1 = players[1]->getMosaicStorage();
    std::shared_ptr<Mosaic> mosaic1 = players[1]->getMosaicStorage()->getMosaic();

    outputString = "Mosaic for " + players[0]->getName() + "\t\t\t" + "Mosaic for " + players[1]->getName() + "\t\t\tMosaic Template:\n"
        + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t" + mStorage1->rowToString(0) + mosaic1->rowToString(0) + "\t" + mosaic1->templateRowToString(0) + "\n"
        + mStorage->rowToString(1) + mosaic->rowToString(1) + "\t" + mStorage1->rowToString(1) + mosaic1->rowToString(1) + "\t" + mosaic1->templateRowToString(1) + "\n"
        + mStorage->rowToString(2) + mosaic->rowToString(2) + "\t" + mStorage1->rowToString(2) + mosaic1->rowToString(2) + "\t" + mosaic1->templateRowToString(2) + "\n"
        + mStorage->rowToString(3) + mosaic->rowToString(3) + "\t" + mStorage1->rowToString(3) + mosaic1->rowToString(3) + "\t" + mosaic1->templateRowToString(3) + "\n"
        + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t" + mStorage1->rowToString(4) + mosaic1->rowToString(4) + "\t" + mosaic1->templateRowToString(4) + "\n";
    
    if(sixBySix){
        outputString +=mStorage->rowToString(5) + mosaic->rowToString(5) + "\t" + mStorage1->rowToString(5) + mosaic1->rowToString(5) + "\t" + mosaic1->templateRowToString(5) + "\n";
        outputString += players[0]->getMosaicStorage()->getBrokenTiles()->toString() + "\t" + players[1]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
        }
    else  outputString += players[0]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t" + players[1]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";

    if( noOfPlayers == 3){
        mStorage = players[2]->getMosaicStorage();
        mosaic = players[2]->getMosaicStorage()->getMosaic();    
        outputString += "Mosaic for " + players[2]->getName() + "\n"
        + mStorage->rowToString(0) + mosaic->rowToString(0) + "\n"
        + mStorage->rowToString(1) + mosaic->rowToString(1) + "\n"
        + mStorage->rowToString(2) + mosaic->rowToString(2) + "\n"
        + mStorage->rowToString(3) + mosaic->rowToString(3) + "\n"
        + mStorage->rowToString(4) + mosaic->rowToString(4) + "\n";
        if(sixBySix){
        outputString +=mStorage->rowToString(5) + mosaic->rowToString(5) + "\n";
        outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
        }
    else  outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
    }

    else if( noOfPlayers == 4){
        mStorage = players[2]->getMosaicStorage();
        mosaic = players[2]->getMosaicStorage()->getMosaic();   
        mStorage1 = players[3]->getMosaicStorage();
        mosaic1 = players[3]->getMosaicStorage()->getMosaic();    
        outputString += "Mosaic for " + players[2]->getName() + "\t\t\t" + "Mosaic for " + players[3]->getName() + "\n"
        + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t" + mStorage1->rowToString(0) + mosaic1->rowToString(0) + "\n"
        + mStorage->rowToString(1) + mosaic->rowToString(1) + "\t" + mStorage1->rowToString(1) + mosaic1->rowToString(1) + "\n"
        + mStorage->rowToString(2) + mosaic->rowToString(2) + "\t" + mStorage1->rowToString(2) + mosaic1->rowToString(2) + "\n"
        + mStorage->rowToString(3) + mosaic->rowToString(3) + "\t" + mStorage1->rowToString(3) + mosaic1->rowToString(3) + "\n"
        + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t" + mStorage1->rowToString(4) + mosaic1->rowToString(4) + "\n";

        if(sixBySix){
        outputString +=mStorage->rowToString(5) + mosaic->rowToString(5) + "\t" + mStorage1->rowToString(5) + mosaic1->rowToString(5) + "\n";
        outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\t" + players[3]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
        }
        else  outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t" + players[3]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
    }
    std::cout << outputString << std::endl;
}

void GameEngineCallback::playerTurnUpdate(std::string playerName) {
    std::cout<< "It is now " << playerName <<"'s turn. " << std::endl;
}

void GameEngineCallback::playerTurnResult(std::string turnResult) {
    std::cout << turnResult << std::endl;
}

void GameEngineCallback::promptUser(std::string prompt) {
    std::cout << prompt << std::endl;
}

void GameEngineCallback::playerEndOfRoundResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers) {
    
    for(int i = 0; i<noOfPlayers; ++i){
        std::cout << players[i]->getName() << " Points: " << players[i]->getPoints() << std::endl;
    }
    
    std::cout<< "\n=====End of round - Beginning next round!=====\n" << std::endl;
 
}

void GameEngineCallback::playerEndOfGameResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers) {
    for(int i = 0; i<noOfPlayers; ++i){
        std::cout << players[i]->getName() << " Points: " << players[i]->getPoints() << std::endl;
    }
    std::cout<< "\n=================End of Game=================" << std::endl;
    int pointsOfWinner = players[0]->getPoints();

    for(int i = 0; i<noOfPlayers; ++i){
        if(players[i]->getPoints() > pointsOfWinner){
            pointsOfWinner = players[i]->getPoints();
        }
    }

    std::vector<int> winners;
    for(int i = 0; i < noOfPlayers; ++i){
        if(players[i]->getPoints() == pointsOfWinner){
            winners.push_back(i);
        }
    }

    if( winners.size() > 1){
        std::cout << "Winners are "; 
        for(unsigned int i = 0; i<winners.size();++i){
            std::cout << players[i]->getName();
            if(i != winners.size()-1){
                std::cout << " and ";
            }
        }
    }else{
        std::cout << "Winner is " << players[winners[0]]->getName(); 
    }
    
    std::cout << "!\n" << std::endl;

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