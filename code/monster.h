#pragma once
#include <string>
#include <map>


class Monster
{
public:

    Monster(std::string name_, int level);
    int GetStat(const std::string &stat);
    std::map<std::string, int> GetStats();

    std::string name;
    int level;
    std::string element;
    std::map<std::string, int> base_stats;
    int level_up{};
    int xp{};
    int health{};
    int energy{};
};
