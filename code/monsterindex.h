#pragma once
#include <vector>
#include <raylib.h>
#include "monster.h"


class MonsterIndex
{
public:

    MonsterIndex(std::vector<Monster> monsters, const std::map<std::string, Font> &fonts);
    void Update(double dt);

private:

    RenderTexture2D tint_surface{};
    std::map<std::string, Font> fonts;
};
