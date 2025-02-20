#ifndef GAME_H
#define GAME_H
#include <cstdint>
#include <string>
#include <vector>

struct Ship {
    int size;
    char direction;
    uint64_t x, y;

    Ship(uint64_t s, char o, uint64_t x_, uint64_t y_);
};

struct Game {
    uint64_t w;
    uint64_t h;
    uint64_t ships[5];
    std::string strategy;
    char** pole; // 0 неизвестно что в клетке 1 в ней корабль 2 в нее стреляли
    uint64_t lastH_ordered = 0;
    uint64_t lastW_ordered = 0;
    uint64_t lastH_custom = h - 1;
    uint64_t lastW_custom = w - 1;
    uint64_t count_ships;
    std::vector<Ship> Ships_struct;

    void start(std::string role);
    void creat_pole(uint64_t w, uint64_t h);
    void bot_ships();
    bool can_place_ship(uint64_t x, uint64_t y, int size, char direction);
    void place_ship(uint64_t x, uint64_t y, int size, char direction);
    void pleyer_shot(uint64_t x, uint64_t y);
    void bot_shot();
    void get_pole();
    bool finished();
    bool remove_ship(const Ship& ship_to_remove);
    bool is_ship_destroyed(uint64_t x, uint64_t y);

};



#endif //GAME_H
