#include "monster.h"


Monster::Monster(const std::string &name, const int level) : name(name), level(level)
{
    // stats
    element = MONSTER_DATA[name].element;
    base_stats = MONSTER_DATA[name].stats;
}
