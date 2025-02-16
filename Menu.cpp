#include <iostream>
#include "Menu.h"
#include<string>
#include<stdlib.h>
#include<exception>

Menu::Menu() {
    this->gameEngine = new GameEngine();
}

Menu::Menu(int seed) {
    this->gameEngine = new GameEngine(seed);
}

Menu::~Menu(){
    delete this->gameEngine;
    this->gameEngine = nullptr;
}

void Menu::printWelcome() {
    std::cout << "\033[1;31m\t\t\t     W E L C O M E    T O " << std::endl;
    std::cout<< ".----------------.  .----------------.  .----------------.  .----------------."<<std::endl;
    std::cout<<"| .--------------. || .--------------. || .--------------. || .--------------. |"<<std::endl;
    std::cout<<"| |      __      | || |   ________   | || | _____  _____ | || |   _____      | |"<<std::endl;
    std::cout<<"| |     /  \\     | || |  |  __   _|  | || ||_   _||_   _|| || |  |_   _|     | |"<<std::endl;
    std::cout<<"| |    / /\\ \\    | || |  |_/  / /    | || |  | |    | |  | || |    | |       | |"<<std::endl;
    std::cout<<"| |   / ____ \\   | || |     .'.' _   | || |  | '    ' |  | || |    | |   _   | |"<<std::endl;
    std::cout<<"| | _/ /    \\ \\_ | || |   _/ /__/ |  | || |   \\ `--' /   | || |   _| |__/ |  | |"<<std::endl;
    std::cout<<"| ||____|  |____|| || |  |________|  | || |    `.__.'    | || |  |________|  | |"<<std::endl;
    std::cout<<"| |              | || |              | || |              | || |              | |"<<std::endl;
    std::cout<<"| '--------------' || '--------------' || '--------------' || '--------------' |"<<std::endl;
    std::cout<<" '----------------'  '----------------'  '----------------\'  \'----------------\' " <<std::endl;
    std::cout << "\033[0m" << std::endl;
}

void Menu::runMenu() {
    std::cout << "\033[1m********************************************************************************" << std::endl;
    std::cout << "Menu" << std::endl;
    std::cout << "********************************************************************************\033[0m" << std::endl;
    std::cout << "1. New Standard Game" << std::endl;
    std::cout << "2. New Grey Board Game" << std::endl;
    std::cout << "3. New 6x6 Game" << std::endl;
    std::cout << "4. Load Game" << std::endl;
    std::cout << "5. Help / Game Information" << std::endl;
    std::cout << "6. Credits (Show Student Information)" << std::endl;
    std::cout << "7. Quit" << std::endl << std::endl;

}

bool Menu::runSelection(unsigned const int selection) {

    // Parameters used for io redirection
    bool eof = false;
    bool continueMenuLoop = true;

    Input input;

    if (selection == OPTIONS::NEW_GAME) {
        std::string playerNames[4] = {};
        int noOfPlayers = 0;
        int noOfFactories = 0;
        determinePlayersAndFactories(playerNames,noOfPlayers,noOfFactories);

        std::string gameMode = "standard";

        this->gameEngine->newGame(playerNames, noOfPlayers, noOfFactories, gameMode);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop);
        

    } else if(selection == OPTIONS::NEW_GREY_GAME){

        int noOfPlayers = 0;
        int noOfFactories = 0;
        std::string playerNames[4] = {};
        determinePlayersAndFactories(playerNames,noOfPlayers,noOfFactories);
        std::string gameMode = "grey";

        this->gameEngine->newGame(playerNames, noOfPlayers, noOfFactories, gameMode);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop);

    } else if(selection == OPTIONS::NEW_6X6_GAME){

        int noOfPlayers = 0;
        int noOfFactories = 0;
        std::string playerNames[4] = {};
        determinePlayersAndFactories(playerNames,noOfPlayers,noOfFactories);
        std::string gameMode = "six";

        this->gameEngine->newGame(playerNames, noOfPlayers, noOfFactories, gameMode);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop);
    } else if (selection == OPTIONS::LOAD_GAME) { 
        std::string file = input.getString();

        GameEngineIO* gameEngineIO = new GameEngineIO(this->gameEngine);
        try{
            gameEngineIO->loadGame(file);

            std::cout << "Azul game successfully loaded" << std::endl;

            this->gameEngine->gameplayLoop(eof, continueMenuLoop); 
        }catch(const char* e){
            std::cerr<< e << std::endl;
        }
        delete gameEngineIO;
    } else if (selection == OPTIONS::HELP_MENU) {
        std::cout << "Play the game by entering the appropriate command into the console." << std::endl;
        std::cout << "At any time during the game, type \"help\" for information on how format the input" << std::endl;
        std::cout << "For game rules please visit: https://www.ultraboardgames.com/azul/game-rules.php" <<std::endl;
    } else if (selection == OPTIONS::CREDITS) {
        std::cout << "------------------------" << std::endl << std::endl;
        printCredits("Ian Nguyen", "s3788210");
        printCredits("Paula Kurniawan", "s3782041");
        printCredits("Josiah Miranda", "s3782051");
        std::cout << "------------------------" << std::endl << std::endl;
    } else if (selection == OPTIONS::QUIT) {
        std::cout << "Goodbye" << std::endl;
    } else {
        std::cout << "Please enter a valid number." << std::endl;
    }

    return continueMenuLoop;
}

void Menu::printCredits(const std::string name, const std::string studentID) {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Student ID: " << studentID << std::endl;
    std::cout << "Email: " << studentID << "@student.rmit.edu.au" << std::endl << std::endl;
}

void Menu::determinePlayersAndFactories(std::string playerNames[], int& noOfPlayer, int& noOfFactories){
    std::cout << "\033[1;38;5;33mPlease enter the number of players:\033[0m" << std::endl;
    std::string noOfPlayerAsString = input.getString();
    if (input.inputIsInt(noOfPlayerAsString)) {
        std::stringstream sstream (noOfPlayerAsString);
        noOfPlayer = 0;
        sstream >> noOfPlayer;
        if(noOfPlayer <= 0 || noOfPlayer >4){
            throw "\033[1;31mPlease enter a number from 1-4.\033[0m";
        }
    } else {
        throw "\033[1;31mPlease enter a number from 1-4.\033[0m";
    }

    for(int i = 0; i < noOfPlayer; ++i){
        std::cout << "\033[1;38;5;33mPlease enter player "<< i+1 <<"'s name:\033[0m" << std::endl;
        playerNames[i] = input.getString();

    }
    
    for(int i = 0; i < noOfPlayer; ++i){
        for(int j = i + 1; j < noOfPlayer; ++j){
            if (playerNames[i] == playerNames[j]) {
                throw "\033[1;31mPlayers cannot have the same name.\033[0m";
            }
        }
    }

    std::cout << "\033[1;38;5;33mPlease enter the number of central factories (1-2):\033[0m" << std::endl;
    std::string noOfFactoriesAsString = input.getString();
    if (input.inputIsInt(noOfFactoriesAsString)) {
        std::stringstream sstream (noOfFactoriesAsString);
        noOfFactories = 0;
        sstream >> noOfFactories;
        if(noOfFactories <= 0 || noOfFactories >2){
            throw "\033[1;31mPlease enter a number from 1-2.\033[0m";
        }
    } else {
        throw "\033[1;31mPlease enter a number from 1-2.\033[0m";
    }

    std::cout << "\033[1;33mWelcome ";
    for(int i = 0; i < noOfPlayer; ++i){
        if(i == noOfPlayer-1)
            std::cout << "and " << playerNames[i];
        else
            std::cout << playerNames[i] << ", ";
    }
    std::cout << "!\033[0m" << std::endl;

    std::cout << "Note: Type \"help\" at any point for information." << std::endl;
}