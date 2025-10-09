#include <array>
#include <rygame.hpp>
#include "monster.hpp"


Monster::Monster(const std::string &name, const int level) : name(name), level(level)
{
    // stats
    element = GameData::GetInstance().MONSTER_DATA[name].element;
    base_stats = GameData::GetInstance().MONSTER_DATA[name].stats;
    abilities = GameData::GetInstance().MONSTER_DATA[name].abilities;

    level_up = level * 150.0f;
    health = base_stats["max_health"] * level;
    energy = base_stats["max_energy"] * level;

    evolution = GameData::GetInstance().MONSTER_DATA[name].evolve;
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
            if (all || GameData::GetInstance().ATTACK_DATA[ability].cost <= energy)
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

void Monster::Update(const float dt)
{
    StatLimiter();
    if (!paused && health > 0)
    {
        initiative += GetStat("speed") * dt;
    }
}

void Monster::ReduceEnergy(const Attack attack)
{
    energy -= GameData::GetInstance().ATTACK_DATA[attack].cost;
}

float Monster::GetBaseDamage(const Attack attack)
{
    return GetStat("attack") * GameData::GetInstance().ATTACK_DATA[attack].amount;
}

void Monster::UpdateXP(const int amount)
{
    if (level_up - xp > amount)
    {
        xp += amount;
    }
    else
    {
        const auto health_perc = health / GetStat("max_health");
        const auto energy_perc = energy / GetStat("max_energy");
        ++level;
        xp = amount - (level_up - xp);
        level_up = level * 150.f;
        health = GetStat("max_health") * health_perc;
        energy = GetStat("max_energy") * energy_perc;
    }
}

void Monster::StatLimiter()
{
    health = rl::Clamp(health, 0, GetStat("max_health"));
    energy = rl::Clamp(energy, 0, GetStat("max_energy"));
}

std::ostream &operator<<(std::ostream &os, Monster const &m)
{
    char level[3]{};
    sprintf(level, "%d", m.level);
    os << std::string("Monster: ") << m.name << std::string(", lvl: ") << std::string(level);
    return os;
}
