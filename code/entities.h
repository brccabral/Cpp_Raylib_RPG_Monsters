#pragma once
#include "sprite.h"


class Player : public Sprite
{
public:

    Player(Vector2 position, Texture2D *image, SpriteGroup *sprite_group, Rectangle imgRect);
};
