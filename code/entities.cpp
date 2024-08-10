#include "entities.h"
#include "settings.h"


Entity::Entity(
        const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames)
    : frames_direction(frames)
{
    image = frames["down"];
    image->atlas_rect = frames["down"]->frames[0];
    rect = image->GetRect();
    rect.center(pos);
}

void Entity::Update(const float deltaTime)
{
    Sprite::Update(deltaTime);
    Animate(deltaTime);
}

void Entity::Animate(const float dt)
{
    frame_index += ANIMATION_SPEED * dt;
    if (frame_index > std::dynamic_pointer_cast<rg::Frames>(image)->frames.size())
    {
        frame_index = 0;
    }
    image->atlas_rect = std::dynamic_pointer_cast<rg::Frames>(image)->frames[int(frame_index)];
}

Player::Player(
        const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames)
    : Entity(pos, frames)
{}

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
    Entity::Update(dt);
    Input();
    Move(dt);
}

void Player::Move(const float dt)
{
    // we split x and y to calculate collisions independently
    rect.x += direction.x * speed * dt;
    rect.y += direction.y * speed * dt;
}
