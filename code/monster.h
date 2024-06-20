#pragma once
#include <string>
#include <map>
#include <vector>
#include <array>


class Monster
{
public:

    Monster(std::string name_, int level);
    float GetStat(const std::string &stat);
    std::vector<std::pair<std::string, float>> GetStats();
    std::vector<std::string> GetAbilities();
    std::array<std::pair<float, float>, 3> GetInfo();
    void Update(double dt);
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
    float initiative{};
    bool paused{};
};
