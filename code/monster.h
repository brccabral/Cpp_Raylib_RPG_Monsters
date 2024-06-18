#pragma once
#include <string>
#include <map>
#include <vector>


class Monster
{
public:

    Monster(std::string name_, int level);
    float GetStat(const std::string &stat);
    std::vector<std::pair<std::string, float>> GetStats();
    std::vector<std::string> GetAbilities();
    friend std::ostream &operator<<(std::ostream &os, Monster const &m);

    std::string name;
    int level;
    std::string element;
    std::map<std::string, float> base_stats;
    std::vector<std::pair<int, std::string>> abilities;
    int level_up{};
    int xp{};
    float health{};
    float energy{};
};
