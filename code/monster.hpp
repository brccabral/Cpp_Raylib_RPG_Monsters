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
    std::vector<Attack> GetAbilities(bool all = true);

    std::string name;
    int level;
    ElementType element;
    std::map<std::string, float> base_stats;
    std::vector<std::pair<int, Attack>> abilities;
    float xp{};
    float level_up{};
    float health{};
    float energy{};
};