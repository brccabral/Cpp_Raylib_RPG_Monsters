#include <utility>
#include "monster.h"
#include "game_data.h"


Monster::Monster(std::string name_, const int level) : name(std::move(name_)), level(level)
{
    element = MONSTER_DATA[name].element;
    base_stats = MONSTER_DATA[name].stats;
}
