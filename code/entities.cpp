#include "entities.h"
#include <raymath.h>

#include <utility>
#include "support.h"


Entity::Entity(const Vector2 pos, tilemap_name fr, SpriteGroup *sg, const Rectangle rect)
    : SimpleSprite(sg), frames(std::move(fr))
{
    position = pos;
    frame_index = 0;
    tiled_texture.texture = &frames["down"][frame_index];
    tiled_texture.rect = {0, 0, 128, 128};
    SpriteType = 3;
}

Player::Player(const Vector2 position, const tilemap_name &frames, SpriteGroup *sprite_group, const Rectangle imgRect)
    : Entity(position, frames, sprite_group, imgRect)
{
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
