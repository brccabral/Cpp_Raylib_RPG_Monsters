#pragma once
#include <string>
#include <map>


class Monster
{
public:

    Monster(std::string name_, int level);
    float GetStat(const std::string &stat);
    std::map<std::string, float> GetStats();

    std::string name;
    int level;
    std::string element;
    std::map<std::string, float> base_stats;
    int level_up{};
    int xp{};
    float health{};
    float energy{};
};
