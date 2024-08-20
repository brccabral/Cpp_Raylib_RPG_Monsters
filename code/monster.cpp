#include <rygame.hpp>
#include "monster.hpp"


Monster::Monster(const std::string &name, const int level) : name(name), level(level)
{
    // stats
    element = MONSTER_DATA[name].element;
    base_stats = MONSTER_DATA[name].stats;
    abilities = MONSTER_DATA[name].abilities;

    level_up = level * 150;
    xp = rl::GetRandomValue(10, level_up);
    health = base_stats["max_health"] * level;
    health -= rl::GetRandomValue(0, 200);
    energy = base_stats["max_energy"] * level;
    energy -= rl::GetRandomValue(0, 100);
}

float Monster::GetStat(const std::string &stat)
{
    return base_stats[stat] * level;
}

std::vector<std::pair<std::string, float>> Monster::GetStats()
{
    return {
            {"health", GetStat("max_health")}, //
            {"energy", GetStat("max_energy")}, //
            {"attack", GetStat("attack")}, //
            {"defense", GetStat("defense")}, //
            {"speed", GetStat("speed")}, //
            {"recovery", GetStat("recovery")}, //
    };
}

// Get abilities based on level
std::vector<Attack> Monster::GetAbilities(const bool all)
{
    std::vector<Attack> result;
    for (auto &[lvl, ability]: abilities)
    {
        if (level >= lvl)
        {
            if (all || ATTACK_DATA[ability].cost < energy)
            {
                result.push_back(ability);
            }
        }
    }
    return result;
}
