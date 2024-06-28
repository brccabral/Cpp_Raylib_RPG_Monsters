#pragma once
#include <string>
#include <map>
#include <vector>
#include <array>
#include "game_data.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

class Monster
{
public:

    Monster(std::string name_, int level);
    float GetStat(const std::string &stat);
    std::vector<std::pair<std::string, float>> GetStats();
    std::vector<Attack> GetAbilities(bool all = true);
    std::array<std::pair<float, float>, 3> GetInfo();
    void Update(double dt);
    void ReduceEnergy(Attack attack);
    float GetBaseDamage(Attack attack);
    void UpdateXP(int amount);
    void StatLimiter();
    friend std::ostream &operator<<(std::ostream &os, Monster const &m);

    std::string name;
    int level;
    ElementType element;
    std::map<std::string, float> base_stats;
    std::vector<std::pair<int, Attack>> abilities;

    // evolution
    int level_up{};
    int xp{};
    std::pair<std::string, int> evolve;

    // stats
    float health{};
    float energy{};
    float initiative{};
    bool paused{};
    // when it is this monster time to action, if already defending, stop defending. When
    // it is being attacked, if defending, reduces damage taken
    bool defending{};
};
