#include "entities.h"
#include <raymath.h>
#include "support.h"


Entity::Entity(const Vector2 pos, Texture2D *tx, SpriteGroup *sg) : SimpleSprite(sg)
{
    position = pos;
    frame_index = 0;
    tiled_texture.texture = tx;
    tiled_texture.rect = {0, 0, 128, 128};
    SpriteType = 3;
}

Player::Player(Vector2 position, Texture2D *tx, SpriteGroup *sprite_group, tilerect_name rects)
    : Entity(position, tx, sprite_group)
{
    tiled_texture.rect = rects["down"][0];
    SpriteType = 2;
}

void Player::Input()
{
    Vector2 input_vector{};
    if (IsKeyDown(KEY_UP))
    {
        input_vector.y -= 1;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        input_vector.y += 1;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        input_vector.x -= 1;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        input_vector.x += 1;
    }
    direction = input_vector;
}

void Player::Move(const double deltaTime)
{
    position = Vector2Add(position, Vector2Scale(direction, 250 * deltaTime));
}

void Player::Update(const double deltaTime)
{
    Input();
    Move(deltaTime);
}

Vector2 Player::GetCenter() const
{
    return {position.x + 64.0f / 2, position.y + 64.0f / 2};
    // return {position.x + imgRect.width / 2, position.y + imgRect.height / 2};
}
