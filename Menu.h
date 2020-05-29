#ifndef MENU_H
#define MENU_H

#include <string>
#include "Input.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/GameEngineIO.h"

enum OPTIONS {
    NO_OPTION,
    NEW_GAME,
    NEW_GREY_GAME,
    NEW_6X6_GAME,
    LOAD_GAME,
    HELP_MENU,
    CREDITS,
    QUIT,
};

class Menu {
public:
    // Constructor
    Menu();

    // Constructor for when they define seed
    Menu(int seed);
    ~Menu();
    // Print the menu.
    void runMenu();

    // Print Welcome Message.
    void printWelcome();

    bool runSelection(unsigned const int selection);
    
    void determinePlayersAndFactories(std::string playerNames[], int& noOfPlayer, int& noOfFactories);

    void printCredits(const std::string name, const std::string studentID);

private:
    // Input for Menu
    Input input;
    GameEngine* gameEngine;
};

#endif // MENU_H