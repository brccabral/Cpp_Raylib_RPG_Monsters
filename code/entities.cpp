#include "entities.h"
#include <raymath.h>

#include <utility>

#include "settings.h"
#include "support.h"


Entity::Entity(
        const Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg,
        std::string facing_dir)
    : SimpleSprite(sg), facing_direction(std::move(facing_dir)), frame_index(0), frames(named_tts)
{
    position = pos;
    frame_index = 0;
    tiled_texture = frames[GetState()][0];
    SpriteType = 3;
}

void Entity::Animate(const double dt)
{
    frame_index += ANIMATION_SPEED * dt;
    tiled_texture = frames[GetState()][int(frame_index) % frames[GetState()].size()];
}

void Entity::Update(const double dt)
{
    Animate(dt);
}

std::string Entity::GetState()
{
    const bool moving = (direction.x || direction.y);
    if (moving)
    {
        if (direction.x)
        {
            facing_direction = direction.x > 0 ? "right" : "left";
        }
        if (direction.y)
        {
            facing_direction = direction.y > 0 ? "down" : "up";
        }
    }
    return facing_direction + (moving ? "" : "_idle");
}

Character::Character(
        const Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg,
        std::string facing_dir)
    : Entity(pos, named_tts, sg, std::move(facing_dir))
{}

Player::Player(
        const Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg,
        std::string facing_dir)
    : Entity(pos, named_tts, sg, std::move(facing_dir))
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
    position = Vector2Add(position, Vector2Scale(direction, speed * deltaTime));
}

void Player::Update(const double deltaTime)
{
    Input();
    Move(deltaTime);
    Animate(deltaTime);
}

Vector2 Player::GetCenter() const
{
    return {position.x + tiled_texture.rect.width / 2, position.y + tiled_texture.rect.height / 2};
}
