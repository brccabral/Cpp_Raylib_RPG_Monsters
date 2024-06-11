#include "entities.h"
#include <raymath.h>

Player::Player(const Vector2 position, Texture2D *image, SpriteGroup *sprite_group, const Rectangle imgRect)
    : Sprite(position, image, sprite_group, imgRect)
{
    SpriteType = 2;
}

void Player::Draw(const Vector2 offset) const
{
    DrawRectangleV(Vector2Add(position, offset), {64, 64}, RED);
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

void Player::Move(double deltaTime)
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
