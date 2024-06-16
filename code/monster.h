#pragma once
#include <string>
#include <map>


class Monster
{
public:

    Monster(std::string name, int level);

private:

    std::string name;
    int level;
    std::string element;
    std::map<std::string, int> base_stats;
};
