#include <utility>
#include "monster.h"
#include "game_data.h"


Monster::Monster(std::string name_, const int level) : name(std::move(name_)), level(level)
{
    element = MONSTER_DATA[name].element;
    base_stats = MONSTER_DATA[name].stats;
    abilities = MONSTER_DATA[name].abilities;

    level_up = level * 150;
    xp = GetRandomValue(0, level_up);
    health = base_stats["max_health"] * level;
    health *= GetRandomValue(0, 100) / 100.0f;
    energy = base_stats["max_energy"] * level;
    energy *= GetRandomValue(0, 100) / 100.0f;
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
std::vector<std::string> Monster::GetAbilities()
{
    std::vector<std::string> result;
    for (auto &[lvl, ability]: abilities)
    {
        if (level >= lvl)
        {
            result.push_back(ability);
        }
    }
    return result;
}

std::array<std::pair<float, float>, 3> Monster::GetInfo()
{
    std::array<std::pair<float, float>, 3> info;
    info[0] = std::make_pair(health, GetStat("max_health"));
    info[1] = std::make_pair(energy, GetStat("max_energy"));
    info[2] = std::make_pair(initiative, 100);
    return info;
}

std::ostream &operator<<(std::ostream &os, Monster const &m)
{
    char level[3];
    sprintf(level, "%d", m.level);
    os << std::string("Monster: ") << m.name << std::string(", lvl: ") << std::string(level);
    return os;
}
