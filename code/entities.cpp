#include "entities.h"

#include <iostream>
#include <raymath.h>

Player::Player(Vector2 position, Texture2D *image, SpriteGroup *sprite_group, Rectangle imgRect)
    : Sprite(position, image, sprite_group, imgRect)
{}

void Player::Draw() const
{
    DrawRectangleV(position, {64, 64}, RED);
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
