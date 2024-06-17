#pragma once
#include <string>
#include <map>


class Monster
{
public:

    Monster(std::string name_, int level);
    int GetStat(const std::string &stat);

    std::string name;
    std::string element;
    int level;
    int xp{};
    int level_up{};
    int health{};
    std::map<std::string, int> base_stats;
};
