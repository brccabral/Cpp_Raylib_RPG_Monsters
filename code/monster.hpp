#pragma once
#include <string>
#include "game_data.hpp"


class Monster
{
public:

    Monster() = default; // need a default constructor for map<>
    Monster(const std::string &name, int level);
    float GetStat(const std::string &stat);
    std::vector<std::pair<std::string, float>> GetStats();

    std::string name;
    int level;
    ElementType element;
    std::map<std::string, float> base_stats;
    float xp{};
    float level_up{};
    float health{};
    float energy{};
};
