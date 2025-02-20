#include <string>
#include <iostream>
#include "Parser.h"
#include "game.h"

int main() {
    Parser parser;
    Game game;
    for (int i=0; i < 5; i++) {
        parser.ships[i] = 0;
    }
    while(true) {
        std::string cmd;
        std::getline(std::cin, cmd);
        if(cmd == "exit"){
            std::cout << "Bye!" << std::endl;
            return 0;
        } else {
            parser.parse(cmd);
        }

    }
    return 0;
}
//set height 10
//set width 10
//set count 4 5
//set strategy custom
//create master
//start

