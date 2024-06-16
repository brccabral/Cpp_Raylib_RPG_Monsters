#include <utility>
#include "monster.h"
#include "game_data.h"


Monster::Monster(std::string name, const int level) : name(std::move(name)), level(level)
{
    element = MONSTER_DATA[name].name;
    base_stats = MONSTER_DATA[name].stats;
}
