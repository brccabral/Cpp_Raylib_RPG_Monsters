#pragma once
#include "sprite.h"


class AllSprites : public SpriteGroup
{
public:

    void Draw(Vector2 player_center);
    Vector2 offset{};
};
