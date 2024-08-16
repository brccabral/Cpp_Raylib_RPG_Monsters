#pragma once
#include "game_data.h"
#include <string>


class Monster
{
public:

    Monster() = default; // need a default constructor for map<>
    Monster(const std::string &name, int level);

    std::string name;
    int level;
    ElementType element;
    std::map<std::string, float> base_stats;
};
