#include "Menu.h"
#include <iostream>
#include "Input.h"
#include <sstream>
#define EXIT_SUCCESS    0

int main(int argc, char** argv) {

    Input input;
    std::shared_ptr<Menu> menu;
    bool error = false;

    // Allows user to input seed to be used for the program
    if (argc >= 3) {
        if (input.inputIsInt(argv[2])) {
            std::stringstream sstream (argv[2]);
            int seed = 0;
            sstream >> seed;
            menu = std::make_shared<Menu>(seed);
        } else {
            std::cout << "Seed must be an integer." << std::endl;
            error = true;
        }
    } else {
        menu = std::make_shared<Menu>();
    }

    if (!error) {
        
        menu->printWelcome();

        
        unsigned int selection = OPTIONS::NO_OPTION;
        bool continueMenuLoop = true;
        while (continueMenuLoop && selection != OPTIONS::QUIT) {
            menu->runMenu();
            selection = input.getInt();
            try{
                continueMenuLoop = menu->runSelection(selection);
            }catch(const char* e){
                std::cerr<< e << std::endl;
            }
        }

    }
    
    return EXIT_SUCCESS;
}