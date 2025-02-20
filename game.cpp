#include "game.h"
#include <ctime>
#include <algorithm>
#include <iostream>

void Game::start(std::string role) {
    if (role == "master") {
        lastH_custom = h - 1;
        lastW_custom = w - 1;
        creat_pole(w, h);
        bot_ships();
    }
    std::cout << "Let's goooooo!!!" << std::endl;
}

void Game::creat_pole(uint64_t w, uint64_t h) {
    pole = new char*[h];
    for (uint64_t i = 0; i < h; ++i) {
        pole[i] = new char[w];
        std::fill(pole[i], pole[i] + w, '.');
    }
}


void Game::bot_ships() {
    srand(time(nullptr));
    for (int size = 0; size < 5; ++size) {
        for (uint64_t count = 0; count < ships[size]; ++count) {
            while (true) {
                char direction = (rand() % 2 == 0) ? 'h' : 'v';
                uint64_t x = rand() % w;
                uint64_t y = rand() % h;
                //std::cout << x << " " << y << " " << direction << std::endl;
                if (can_place_ship(x, y, size, direction)) {
                    place_ship(x, y, size, direction);
                    break;
                }
            }
        }
    }
}

bool Game::can_place_ship(uint64_t x, uint64_t y, int size, char direction) {
    if (direction == 'h') {
        if (x + size > h ) return false;
        for (int i = 0; i < size; ++i) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int nx = x + i + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < h && ny >= 0 && ny < w) {
                        if (pole[nx][ny] != '.') return false;
                    } else {
                        return false;
                    }
                }
            }
        }
    } else {
        if (y + size > w) return false;
        for (int i = 0; i < size; ++i) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int nx = x + dx;
                    int ny = y + i + dy;
                    if (nx >= 0 && nx < h && ny >= 0 && ny < w) {
                        if (pole[nx][ny] != '.') return false;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

Ship::Ship(uint64_t size, char orientation, uint64_t x, uint64_t y)
    : size(size), direction(orientation), x(x), y(y) {}

void Game::place_ship(uint64_t x, uint64_t y, int size, char direction) {
    Ships_struct.push_back(Ship(size, direction, x, y));
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t nx = (direction == 'v') ? x : x + i;
        uint64_t ny = (direction == 'v') ? y + i : y;
        pole[nx][ny] = 'S';
    }
}

void Game::pleyer_shot(uint64_t x, uint64_t y) {
    if (x < w && y < h) {
        if (pole[y][x] == '.') {
            pole[y][x] = '*';
            std::cout << "miss\n";
        } else if (pole[y][x] == '1') {
            pole[y][x] = '#';
            if (is_ship_destroyed(x, y)) {
                std::cout << "destroyed\n";
            } else {
                std::cout << "hit\n";
            }
        } else {
            std::cout << "already shot here\n";
        }
    } else {
        std::cout << "wrong shot (x or y too big)\n";
    }
}


bool Game::is_ship_destroyed(uint64_t x, uint64_t y) {
    for (const auto& ship : Ships_struct) {
        if (ship.direction == 'v') {
            if (x == ship.x && y >= ship.y && y < ship.y + ship.size) {
                for (int i = 0; i < ship.size; i++) {
                    if (pole[ship.y + i][x] == '1') {
                        return false;
                    }
                }
                remove_ship(ship);
                std::cout << "Count of ships: " << Ships_struct.size() << "\n";
                return true;
            }
        } else if (ship.direction == 'h') {
            if (y == ship.y && x >= ship.x && x < ship.x + ship.size) {
                for (int i = 0; i < ship.size; i++) {
                    if (pole[y][ship.x + i] == '1') {
                        return false;
                    }
                }
                remove_ship(ship);
                std::cout << "Count of ships: " << Ships_struct.size() << "\n";
                return true;
            }
        }
    }
    return false;
}

bool Game::remove_ship(const Ship& ship_to_remove) {
    auto it = std::remove_if(Ships_struct.begin(), Ships_struct.end(),
                             [&ship_to_remove](const Ship& ship) {
                                 return ship.x == ship_to_remove.x &&
                                        ship.y == ship_to_remove.y &&
                                        ship.size == ship_to_remove.size &&
                                        ship.direction == ship_to_remove.direction;
                             });
    if (it != Ships_struct.end()) {
        Ships_struct.erase(it, Ships_struct.end());
        std::cout << "Ship removed. Count of ships: " << Ships_struct.size() << "\n";
        return true;
    }
    return false;
}
void Game::bot_shot() {
    if (strategy == "ordered") {
        std::cout<< lastH_ordered << " " << lastW_ordered << std::endl;
        lastW_ordered+=1;
        if (lastW_ordered == w) {
            lastH_ordered+=1;
            lastW_ordered = 0;
        }
    } else {
        std::cout<< lastH_custom << " " << lastW_custom << std::endl;
        lastW_custom-=1;
        if (lastW_custom == -1) {
            lastH_custom-=1;
            lastW_custom = w - 1;
        }
    }
}

void Game::get_pole() {
    for (uint64_t i = 0; i < h; ++i) {
        for (uint64_t j = 0; j < w; ++j) {
            std::cout << pole[i][j] << " ";
        }
        std::cout << "\n";
    }
}

bool Game::finished() {
    if (count_ships != 0){
        return false;
    }
    return true;
}




