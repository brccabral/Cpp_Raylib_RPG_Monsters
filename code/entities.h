#pragma once
#include "sprite.h"


class Player : public Sprite
{
public:

    Player(Vector2 position, Texture2D *image, SpriteGroup *sprite_group, Rectangle imgRect);
    void Draw(Vector2 offset) const override;
    void Input();
    void Move(double deltaTime);
    void Update(double deltaTime) override;
    Vector2 direction;
};
