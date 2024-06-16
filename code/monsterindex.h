#pragma once
#include <vector>
#include <raylib.h>
#include "monster.h"


class MonsterIndex
{
public:

    MonsterIndex(std::vector<Monster> monsters, std::vector<Font> fonts);

private:

    RenderTexture2D display_surface{};
    std::vector<Font> fonts;
};
