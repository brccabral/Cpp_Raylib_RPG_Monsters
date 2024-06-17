#pragma once
#include <string>
#include <map>


class Monster
{
public:

    Monster(std::string name_, int level);
    std::string name;
    std::string element;
    int level;

private:

    std::map<std::string, int> base_stats;
};
