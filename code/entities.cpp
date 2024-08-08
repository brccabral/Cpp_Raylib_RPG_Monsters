#include "entities.h"

Player::Player(const rg::math::Vector2 pos)
{
    image = std::make_shared<rg::Surface>(100, 100);
    image->Fill(rl::RED);
    rect = image->GetRect();
    rect.center(pos);
}

void Player::Input()
{
    rg::math::Vector2 input_vector{};
    if (IsKeyDown(rl::KEY_UP))
    {
        input_vector.y -= 1;
    }
    if (IsKeyDown(rl::KEY_DOWN))
    {
        input_vector.y += 1;
    }
    if (IsKeyDown(rl::KEY_LEFT))
    {
        input_vector.x -= 1;
    }
    if (IsKeyDown(rl::KEY_RIGHT))
    {
        input_vector.x += 1;
    }
    direction = input_vector.normalize();
}

void Player::Update(const float dt)
{
    Input();
    Move(dt);
}

void Player::Move(const float dt)
{
    // we split x and y to calculate collisions independently
    rect.x += direction.x * speed * dt;
    rect.y += direction.y * speed * dt;
}
