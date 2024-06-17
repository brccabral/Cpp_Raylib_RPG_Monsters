#include <utility>
#include "monster.h"
#include "game_data.h"


Monster::Monster(std::string name_, const int level) : name(std::move(name_)), level(level)
{
    element = MONSTER_DATA[name].element;
    base_stats = MONSTER_DATA[name].stats;

    level_up = level * 150;
    xp = GetRandomValue(0, level_up);
    health = base_stats["max_health"] * level;
    health *= GetRandomValue(0, 100) / 100.0f;
    energy = base_stats["max_energy"] * level;
    energy *= GetRandomValue(0, 100) / 100.0f;
}

int Monster::GetStat(const std::string &stat)
{
    return base_stats[stat] * level;
}
