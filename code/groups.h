#pragma once
#include "entities.h"
#include "sprite.h"


class AllSprites : public SpriteGroup
{
public:

    AllSprites();
    ~AllSprites() override;

    void Draw(const Player *player);
    Vector2 offset{};

private:

    Texture2D shadow{};
    Texture2D notice{};
};
