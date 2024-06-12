#pragma once
#include "sprite.h"


class AllSprites : public SpriteGroup
{
public:

    AllSprites();
    ~AllSprites();

    void Draw(Vector2 player_center);
    Vector2 offset{};

private:

    Texture2D shadow{};
};
