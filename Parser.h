#ifndef PARSER_H
#define PARSER_H
#include <cstdint>
#include <string>

struct Parser {
    std::string role;
    std::string strategy;
    uint64_t height = 0, width = 0;
    uint64_t ships[5];

    void parse(std::string cmd);
    bool can_place();
    bool all_arg();
};

#endif //PARSER_H
