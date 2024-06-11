#include "entities.h"

Player::Player(Vector2 position, Texture2D *image, SpriteGroup *sprite_group, Rectangle imgRect)
    : Sprite(position, image, sprite_group, imgRect)
{}
