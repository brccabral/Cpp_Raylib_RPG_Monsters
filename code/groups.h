#pragma once
#include "sprite.h"


class AllSprites : public SpriteGroup
{
public:

    void Draw() const;
    Vector2 offset{100, 20};
};
