#include "monster.h"


Monster::Monster(const std::string &name, const int level) : name(name), level(level)
{
    // stats
    element = MONSTER_DATA[name].element;
    base_stats = MONSTER_DATA[name].stats;

    level_up = level * 150;
    // xp = rl::GetRandomValue(10, level_up);
}
