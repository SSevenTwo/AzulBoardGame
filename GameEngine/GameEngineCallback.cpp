#include "GameEngineCallback.h"

GameEngineCallback::GameEngineCallback() {

}

GameEngineCallback::~GameEngineCallback() {

}

void GameEngineCallback::playerBoardUpdate(std::vector<std::shared_ptr<Player>> players, 
    int noOfPlayers, bool sixBySix, bool greyMode) {

    std::string outputString = "";

    playerBoardsFor2Players(players, noOfPlayers, sixBySix, greyMode, outputString);
    if( noOfPlayers == 3)
        playerBoardsFor3Players(players, noOfPlayers, sixBySix, outputString);
    else if( noOfPlayers == 4)
        playerBoardsFor4Players(players, noOfPlayers, sixBySix, outputString);
    
    std::cout << outputString << std::endl;
}

void GameEngineCallback::playerTurnUpdate(std::string playerName) {
    std::cout<< "\033[1;38;5;33mIt is now " << playerName <<"'s turn. \033[0m" << std::endl;
}

void GameEngineCallback::playerTurnResult(std::string turnResult) {
    if(turnResult != "Error: Invalid Command.\n")
        std::cout <<"\033[1;32m"<< turnResult <<"\033[0m"<< std::endl;
    else
        std::cout <<"\033[1;31m"<< turnResult <<"\033[0m"<< std::endl;
}

void GameEngineCallback::promptUser(std::string prompt) {
    if(prompt != "Error: Please enter a number from 1-5 or B.\nRemember: Columns and Rows can only have 1 of each color.\n")
        std::cout <<"\033[1;38;5;33m"<< prompt <<"\033[0m"<< std::endl;
    else
        std::cout <<"\033[1;31m"<< prompt <<"\033[0m"<< std::endl;

}

void GameEngineCallback::playerEndOfRoundResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers) {
    
    for(int i = 0; i<noOfPlayers; ++i){
        std::cout << "\033[1m" << players[i]->getName() << " Points: " << players[i]->getPoints() << "\033[0m" << std::endl;
    }
    
    std::cout<< "\n\033[1;38;5;208m=====End of round - Beginning next round!=====\033[0m\n" << std::endl;
 
}

void GameEngineCallback::playerEndOfGameResult(std::vector<std::shared_ptr<Player>> players, int noOfPlayers) {
    for(int i = 0; i<noOfPlayers; ++i){
        std::cout << players[i]->getName() << "\033[1m Points: " << players[i]->getPoints() << "\033[0m" << std::endl;
    }
    std::cout<< "\n\033[1;38;5;208m\n=================End of Game=================\033[0m\n" << std::endl;
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
        std::cout << "\033[1;33mWinners are "; 
        for(unsigned int i = 0; i<winners.size();++i){
            std::cout << players[i]->getName();
            if(i != winners.size()-1){
                std::cout << " and ";
            }
        }
    }else{
        std::cout << "\033[1;33mWinner is " << players[winners[0]]->getName(); 
    }
    
    std::cout << "!\n\033[0m" << std::endl;

}

void GameEngineCallback::boardComponentUpdate(std::vector<std::shared_ptr<Factory>> factories, bool use2ndFactory) {

    std::cout<< "\033[1;38;5;208m--------------------Next Turn------------------\n\033[0m" << std::endl;
    std::string outputString = "";
    int noOfCenterFactories = 1;
    if(use2ndFactory)
        noOfCenterFactories = 2;

    for(int i = 0; i < noOfCenterFactories; ++i){

        std::vector<std::shared_ptr<Tile>> tilesOfCenter = factories[i]->getAllTiles();
        outputString += "\033[1mFactory " + std::to_string(i) + " (Center): \033[0m";
        int size = tilesOfCenter.size();
        for(int j = 0; j < size; ++j){
            outputString += colouredLetters(tilesOfCenter[j]->getColourType());
        }
        outputString += "\n";
    }
   
    if(factories.size() > 9)
        printComponentsFor4Players(factories,noOfCenterFactories,outputString);
    else if(factories.size() == 8 || factories.size() == 9)
        printComponentsFor3Players(factories,noOfCenterFactories,outputString);
    else
        printComponentsFor2Players(factories,noOfCenterFactories,outputString);

    std::cout << outputString << std::endl;
}

void GameEngineCallback::appendFactory(std::vector<std::shared_ptr<Factory>> factories, 
    int index, std::string& outputString){

    std::vector<std::shared_ptr<Tile>> tiles = factories[index]->getAllTiles();
        outputString += "\033[1mFactory " + std::to_string(index) += ": \033[0m";
        int size = tiles.size();
        for(int j = 0; j < size; ++j){
            outputString += (colouredLetters(tiles[j]->getColourType()));
        }

        for(int i = 0; i<(4-size); ++i){
            outputString += " . ";
        }
            
            
}


void GameEngineCallback::printComponentsFor4Players(std::vector<std::shared_ptr<Factory>> factories, 
    int noOfCentralFactories, std::string& outputString){

    int index = noOfCentralFactories;
    appendFactory(factories, index, outputString);
    outputString += "\t\t";
    appendFactory(factories, (index+5), outputString);
    outputString += "\n";

    appendFactory(factories, ++index, outputString);
    outputString += "\t\t";
    appendFactory(factories, (index+5), outputString);
    outputString += "\n";

    appendFactory(factories, ++index, outputString);
    outputString += "\t\t";
    appendFactory(factories, (index+5), outputString);
    outputString += "\n";

    appendFactory(factories, ++index, outputString);
    outputString += "\t\t";
    appendFactory(factories, (index+5), outputString);
    outputString += "\n";

    appendFactory(factories, (++index), outputString);
    outputString += "\n";

}

void GameEngineCallback::printComponentsFor3Players(std::vector<std::shared_ptr<Factory>> factories, 
    int noOfCentralFactories, std::string& outputString){

    int index = noOfCentralFactories;
    appendFactory(factories, index, outputString);
    outputString += "\t\t";
    appendFactory(factories, (index+4), outputString);
    outputString += "\n";

    appendFactory(factories, ++index, outputString);
    outputString += "\t\t";
    appendFactory(factories, (index+4), outputString);
    outputString += "\n";

    appendFactory(factories, ++index, outputString);
    outputString += "\t\t";
    appendFactory(factories, (index+4), outputString);
    outputString += "\n";

    appendFactory(factories, (++index), outputString);
    outputString += "\n";

}

void GameEngineCallback::printComponentsFor2Players(std::vector<std::shared_ptr<Factory>> factories, 
    int noOfCentralFactories, std::string& outputString){

    for(unsigned int i = (noOfCentralFactories); i < factories.size(); i++){
        std::vector<std::shared_ptr<Tile>> tiles = factories[i]->getAllTiles();
        outputString += "\033[1mFactory " + std::to_string(i) += ": \033[0m";
        int size = tiles.size();
        for(int j = 0; j < size; ++j){
            outputString += colouredLetters(tiles[j]->getColourType());
            outputString += " ";
        }

        outputString += "\n";
    }
}

void GameEngineCallback::playerBoardsFor4Players(std::vector<std::shared_ptr<Player>> players, int noOfPlayers, 
    bool sixBySix, std::string& outputString){

        std::shared_ptr<MosaicStorage> mStorage = players[2]->getMosaicStorage();
        std::shared_ptr<Mosaic>mosaic = players[2]->getMosaicStorage()->getMosaic();   
        std::shared_ptr<MosaicStorage>mStorage1 = players[3]->getMosaicStorage();
        std::shared_ptr<Mosaic>mosaic1 = players[3]->getMosaicStorage()->getMosaic(); 

        if(sixBySix)
            outputString += "\033[1;37mMosaic for " + players[2]->getName() + "\t\t\t\t\t" 
                + "Mosaic for " + players[3]->getName() + "\033[0m\n";
        else
            outputString += "\033[1;37mMosaic for " + players[2]->getName() + "\t\t\t\t" 
                + "Mosaic for " + players[3]->getName() + "\033[0m\n";

        for(int i = 0; i < 5; ++i){
            outputString += mStorage->rowToString(i) + mosaic->rowToString(i) + "\t" 
                + mStorage1->rowToString(i) + mosaic1->rowToString(i) + "\n";
        }

        if(sixBySix){
            outputString +=mStorage->rowToString(5) + mosaic->rowToString(5) 
                + "\t" + mStorage1->rowToString(5) + mosaic1->rowToString(5) + "\n";
            outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t\t" 
                + players[3]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
        }
        else  outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t\t" 
            + players[3]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
}

void GameEngineCallback::playerBoardsFor3Players(std::vector<std::shared_ptr<Player>> players, int noOfPlayers, 
    bool sixBySix, std::string& outputString){

    std::shared_ptr<MosaicStorage> mStorage = players[2]->getMosaicStorage();
    std::shared_ptr<Mosaic> mosaic = players[2]->getMosaicStorage()->getMosaic();    
    outputString += "\033[1;37mMosaic for " + players[2]->getName() + "\033[0m\n";

    for(int i = 0; i < 5; ++i){
        outputString+= mStorage->rowToString(i) + mosaic->rowToString(i) + "\n";
    }

    if(sixBySix){
        outputString +=mStorage->rowToString(5) + mosaic->rowToString(5) + "\n";
        outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
    }else 
        outputString += players[2]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";

}

void GameEngineCallback::playerBoardsFor2Players(std::vector<std::shared_ptr<Player>> players, int noOfPlayers, 
    bool sixBySix, bool greyMode, std::string& outputString){

    std::shared_ptr<MosaicStorage> mStorage = players[0]->getMosaicStorage();
    std::shared_ptr<Mosaic> mosaic = players[0]->getMosaicStorage()->getMosaic();    
    std::shared_ptr<MosaicStorage> mStorage1 = players[1]->getMosaicStorage();
    std::shared_ptr<Mosaic> mosaic1 = players[1]->getMosaicStorage()->getMosaic();

    if(sixBySix)
        outputString += "\033[1;37mMosaic for " + players[0]->getName() + "\t\t\t\t\t" + "Mosaic for " 
            + players[1]->getName() + "\t\t\t\t\tMosaic Template:\033[0m\n";
    else if(greyMode)
        outputString += "\033[1;37mMosaic for " + players[0]->getName() + "\t\t\t\t" + "Mosaic for " 
            + players[1]->getName() + "\033[0m\n";
    else
        outputString += "\033[1;37mMosaic for " + players[0]->getName() + "\t\t\t\t" + "Mosaic for " 
            + players[1]->getName() + "\t\t\t\tMosaic Template:\033[0m\n";

    for(int i = 0; i < 5; ++i){
        outputString += mStorage->rowToString(i) + mosaic->rowToString(i) + "\t" + mStorage1->rowToString(i) 
            + mosaic1->rowToString(i) + "\t" + mosaic1->templateRowToString(i) + "\n";
    }    
    
    if(sixBySix){
        outputString +=mStorage->rowToString(5) + mosaic->rowToString(5) + "\t" + mStorage1->rowToString(5) 
            + mosaic1->rowToString(5) + "\t" + mosaic1->templateRowToString(5) + "\n";
        outputString += players[0]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t\t" 
            + players[1]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
        }
    else  outputString += players[0]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t\t" 
        + players[1]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";

}

std::string GameEngineCallback::colouredLetters(char type){
    std::string toReturn = "";

    if(type == 'R')
        toReturn = "\033[1;31m R \033[0m";
    if(type == 'Y')
        toReturn = "\033[1;30;33m Y \033[0m";
    if(type == 'B')
        toReturn = "\033[1;38;5;27m B \033[0m";
    if(type == 'L')
        toReturn = "\033[1;36m L \033[0m";
    if(type == 'U')
        toReturn = "\033[1m U \033[0m";
    if(type == 'O')
        toReturn = "\033[1;38;5;208m O \033[0m";
    if(type == 'F')
        toReturn = "\033[1;32m F \033[0m";
    
    return toReturn;
}
