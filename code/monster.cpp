#include <sstream>
#include <utility>
#include "monster.h"
#include "game_data.h"

#include <raymath.h>


Monster::Monster(std::string name_, const int level) : name(std::move(name_)), level(level)
{
    element = MONSTER_DATA[name].element;
    base_stats = MONSTER_DATA[name].stats;
    abilities = MONSTER_DATA[name].abilities;
    evolve = MONSTER_DATA[name].evolve;

    level_up = level * 150;
    health = base_stats["max_health"] * level;
    energy = base_stats["max_energy"] * level;
    initiative = 0;
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

std::array<std::pair<float, float>, 3> Monster::GetInfo()
{
    std::array<std::pair<float, float>, 3> info;
    info[0] = std::make_pair(health, GetStat("max_health"));
    info[1] = std::make_pair(energy, GetStat("max_energy"));
    info[2] = std::make_pair(initiative, 100.0f);
    return info;
}

void Monster::Update(const double dt)
{
    StatLimiter();
    if (!paused)
    {
        initiative += GetStat("speed") * dt;
    }
}

void Monster::ReduceEnergy(const Attack attack)
{
    energy -= ATTACK_DATA[attack].cost;
}

float Monster::GetBaseDamage(const Attack attack)
{
    return GetStat("attack") * ATTACK_DATA[attack].amount;
}

void Monster::UpdateXP(const int amount)
{
    if (level_up - xp > amount)
    {
        xp += amount;
    }
    else
    {
        ++level;
        xp = amount - (level_up - xp);
        level_up = level * 150;
    }
}

// avoids showing negative numbers for health and energy
void Monster::StatLimiter()
{
    health = Clamp(health, 0, GetStat("max_health"));
    energy = Clamp(energy, 0, GetStat("max_energy"));
}

std::ostream &operator<<(std::ostream &os, Monster const &m)
{
    char level[3];
    sprintf(level, "%d", m.level);
    os << std::string("Monster: ") << m.name << std::string(", lvl: ") << std::string(level);
    return os;
}
