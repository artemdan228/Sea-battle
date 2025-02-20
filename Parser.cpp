#include "Parser.h"
#include "game.h"
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

Game game;
void Parser::parse(std::string cmd) {
    if (cmd == "ping") {
        std::cout << "pong" << std::endl;
    } else if (cmd.substr(0, 6) == "create") {
        if (cmd.substr(7) == "slave") {
            role = "slave";
            std::cout << "ok" << std::endl;
        } else if (cmd.substr(7) == "master") {
            role = "master";
            std::cout << "ok" << std::endl;
        } else {
            std::cout << "wrong command" << std::endl;
        }
    } else if (cmd.substr(0, 9) == "set width") {
        width = std::stoi(cmd.substr(10));
        if (width < pow(2, 64)) {
            game.w = width;
            std::cout << "ok" << std::endl;
        } else {
            std::cout << "failed" << std::endl;
        }
    } else if (cmd.substr(0, 10) == "set height") {
        height = std::stoi(cmd.substr(10));
        if (height <= pow(2, 64) - 1) {
            game.h = height;
            std::cout << "ok" << std::endl;
        } else {
            std::cout << "failed" << std::endl;
        }
    } else if (cmd.substr(0, 9) == "set count") {
        if (height==0 || width==0) {
            std::cout << "before input height and width" << std::endl;
        } else {
            if (std::stoi(cmd.substr(10, 11))>4) {
                std::cout << "max len 4" << std::endl;
            } else {
                ships[std::stoi(cmd.substr(10, 11))] = std::stoi(cmd.substr(12));
                if (can_place() && (width >= std::stoi(cmd.substr(10, 11)) || height >= std::stoi(cmd.substr(10, 11)))) { /// не брал второе условие
                    for (int i = 0; i < 5; i++) {
                        game.ships[i] = ships[i];
                        game.count_ships += ships[i];
                    }
                    std::cout << "ok" << std::endl;
                } else {
                    ships[std::stoi(cmd.substr(10, 11))] = 0;
                    std::cout << "failed" << std::endl;
                }
            }
        }
    } else if (cmd.substr(0, 12) == "set strategy") {
        if (cmd.substr(13)=="ordered") {
            strategy = cmd.substr(13);
            game.strategy = cmd.substr(13);
            std::cout << "ok" << std::endl;
        } else if (cmd.substr(13)=="custom") {
            strategy = cmd.substr(13);
            game.strategy = cmd.substr(13);
            std::cout << "ok" << std::endl;
        } else {
            std::cout << "wrong command" << std::endl;
        }
    } else if (cmd == "start") {
        if (role == "slave") {
            game.start(role);
        } else if (all_arg()) {
            game.start(role);
        } else {
            std::cout << "can't start game" << std::endl;
        }
    } else if (cmd.substr(0, 4) == "shot") {
        if (cmd.length() == 4) {
            game.bot_shot();
        } else {
            std::istringstream iss(cmd.substr(5));
            uint64_t x, y;
            iss >> x >> y;
            if (x>=height || y>=width) {
                std::cout << "failed" << std::endl;
            }else {
                game.pleyer_shot(x, y);
            }
        }
    } else if (cmd.substr(0, 10) == "set result") {
        std::cout << "ok" << std::endl;
    } else if (cmd == "get width") {
        std::cout << width << std::endl;
    } else if (cmd == "get height") {
        std::cout << height << std::endl;
    } else if (cmd.substr(0, 9) == "get count") {
        std::cout << ships[std::stoi(cmd.substr(10))] << std::endl;
    } else if (cmd == "get_pole") {
        game.get_pole();
    } else if(cmd == "stop"){
        for (int i = 0; i < game.h; ++i) {
            delete[] game.pole[i];
        }
        delete[] game.pole;
        std::cout << "ok" << std::endl;
    } else if(cmd == "finished") {
        if(game.finished()) {
            std::cout << "yes" << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }
    } else if (cmd == "win") {
        if(game.finished()) {
            std::cout << "yes" << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }
    } else if (cmd == "lose") {
        if(game.finished()) {
            std::cout << "no" << std::endl;
        } else {
            std::cout << "yes" << std::endl;
        }
    } else if (cmd.substr(0, 4) == "load") {
        std::string path = cmd.substr(5);
        std::ifstream inFile(path);
        if (!inFile.is_open()) {
            std::cout << "failed" << std::endl;
        }else {
            inFile >> width >> height;
            game.h = height;
            game.w = width;
            game.creat_pole(height, width);
            int size, x, y;
            char direction;
            while (inFile >> size >> direction >> x >> y) {
                game.count_ships += 1;
                game.place_ship(x, y, size, direction);
            }
            std::cout << "ok" << std::endl;
        }
        inFile.close();
    } else if(cmd.substr(0, 4) == "dump"){
        std::string path = cmd.substr(5);
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "failed" << path << "\n";
        } else {
            file << width << " " << height << "\n";
            for (const auto& ship : game.Ships_struct) {
                file << ship.size << " " << ship.direction << " " << ship.x << " " << ship.y << "\n";
            }
            file.close();
            std::cout << "ok"<< "\n";
        }
    } else {
        std::cout << "wrong command" << std::endl;
    }
}

bool Parser::can_place() {
    size_t S = height * width;
    size_t Ships = 0;
    for (size_t i = 0; i < 5; i++) {
        Ships += ships[i] * i;
        Ships += i + 1;
    }
    return S > Ships;
}

bool Parser::all_arg() {
    bool all_present = true;

    if (height <= 0) {
        std::cout << "Height is missing" << std::endl;
        all_present = false;
    }

    if (width <= 0) {
        std::cout << "Width is missing" << std::endl;
        all_present = false;
    }

    bool ships_present = false;
    for (int i = 0; i < 5; ++i) {
        if (ships[i] > 0) {
            ships_present = true;
            break;
        }
    }
    if (!ships_present) {
        std::cout << "Ships are missing" << std::endl;
        all_present = false;
    }

    if (role.empty()) {
        std::cout << "Role is missing" << std::endl;
        all_present = false;
    }

    if (strategy.empty()) {
        std::cout << "Strategy is missing" << std::endl;
        all_present = false;
    }

    if (all_present) {
        return true;
    } else {
        return false;
    }
}


