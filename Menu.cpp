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
    std::cout << "********************" << std::endl;
    std::cout << "Welcome to Azul!" << std::endl;
    std::cout << "********************" << std::endl;
}

void Menu::runMenu() {

    std::cout << "\033[1;31mbold red text\033[0m\n" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "Menu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New Standard Game" << std::endl;
    std::cout << "2. New Grey Board Game" << std::endl;
    std::cout << "3. New 6x6 Game" << std::endl;
    std::cout << "4. Load Game" << std::endl;
    std::cout << "5. Credits (Show Student Information)" << std::endl;
    std::cout << "6. Quit" << std::endl << std::endl;

}

bool Menu::runSelection(unsigned const int selection) {

    // Parameters used for io redirection
    bool eof = false;
    bool continueMenuLoop = true;

    Input input;

    if (selection == OPTIONS::NEW_GAME) {


        std::cout << "Please enter the number of players:" << std::endl;
        std::string noOfPlayerAsString = input.getString();
        int noOfPlayer = 0;
        if (input.inputIsInt(noOfPlayerAsString)) {
            std::stringstream sstream (noOfPlayerAsString);
            noOfPlayer = 0;
            sstream >> noOfPlayer;
            if(noOfPlayer <= 0 || noOfPlayer >4){
                throw "Please enter a number from 1-4.";
            }
        } else {
            throw "Please enter a number from 1-4.";
        }

        std::string playerNames[4] = {};

        for(int i = 0; i < noOfPlayer; ++i){
            std::cout << "Please enter player "<< i+1 <<"'s name:" << std::endl;
            playerNames[i] = input.getString();

        }
        
        for(int i = 0; i < noOfPlayer; ++i){
            for(int j = i + 1; j < noOfPlayer; ++j){
                if (playerNames[i] == playerNames[j]) {
                    throw "Players cannot have the same name.";
                }
            }
        }

        std::cout << "Please enter the number of central factories (1-2):" << std::endl;
        std::string noOfFactoriesAsString = input.getString();
        int noOfFactories = 0;
        if (input.inputIsInt(noOfFactoriesAsString)) {
            std::stringstream sstream (noOfFactoriesAsString);
            noOfFactories = 0;
            sstream >> noOfFactories;
            if(noOfFactories <= 0 || noOfFactories >2){
                throw "Please enter a number from 1-2.";
            }
        } else {
            throw "Please enter a number from 1-2.";
        }

        for(int i = 0; i < noOfPlayer; ++i){
            std::cout << "Welcome " << playerNames[i] << "!" << std::endl;
        }

        std::string gameMode = "standard";

        this->gameEngine->newGame(playerNames, noOfPlayer, noOfFactories, gameMode);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop);
        

    } else if(selection == OPTIONS::NEW_GREY_GAME){

        //std::cout << "Please enter the number of players:" << std::endl;
        //std::string noOfPlayerAsString = input.getString();
        int noOfPlayer = 2; // maybe change this later
        // if (input.inputIsInt(noOfPlayerAsString)) {
        //     std::stringstream sstream (noOfPlayerAsString);
        //     noOfPlayer = 0;
        //     sstream >> noOfPlayer;
        //     if(noOfPlayer <= 0 || noOfPlayer >4){
        //         throw "Please enter a number from 1-4.";
        //     }
        // } else {
        //     throw "Please enter a number from 1-4.";
        // }

        std::string playerNames[4] = {};

        for(int i = 0; i < noOfPlayer; ++i){
            std::cout << "Please enter player "<< i+1 <<"'s name:" << std::endl;
            playerNames[i] = input.getString();

        }
        
        for(int i = 0; i < noOfPlayer; ++i){
            for(int j = i + 1; j < noOfPlayer; ++j){
                if (playerNames[i] == playerNames[j]) {
                    throw "Players cannot have the same name.";
                }
            }
        }

        //std::cout << "Please enter the number of central factories (1-2):" << std::endl;
        //std::string noOfFactoriesAsString = input.getString();
        int noOfFactories = 1; // maybe change this later
        // if (input.inputIsInt(noOfFactoriesAsString)) {
        //     std::stringstream sstream (noOfFactoriesAsString);
        //     noOfFactories = 0;
        //     sstream >> noOfFactories;
        //     if(noOfFactories <= 0 || noOfFactories >2){
        //         throw "Please enter a number from 1-2.";
        //     }
        // } else {
        //     throw "Please enter a number from 1-2.";
        // }

        for(int i = 0; i < noOfPlayer; ++i){
            std::cout << "Welcome " << playerNames[i] << "!" << std::endl;
        }
        std::string gameMode = "grey";

        this->gameEngine->newGame(playerNames, noOfPlayer, noOfFactories, gameMode);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop);

    } else if(selection == OPTIONS::NEW_6X6_GAME){

    } else if (selection == OPTIONS::LOAD_GAME) { 
        std::string file = input.getString();

        GameEngineIO* gameEngineIO = new GameEngineIO(this->gameEngine);
        try{
            gameEngineIO->loadGame(file);
            // this->gameEngine->loadGame(file);

            std::cout << "Azul game successfully loaded" << std::endl;

            this->gameEngine->gameplayLoop(eof, continueMenuLoop); 
        }catch(const char* e){
            std::cerr<< e << std::endl;
        }
        delete gameEngineIO;
        
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

