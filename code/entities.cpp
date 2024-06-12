#include "entities.h"
#include <raymath.h>
#include "support.h"


Entity::Entity(const Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg)
    : SimpleSprite(sg), frame_index(0), frames(named_tts)
{
    position = pos;
    frame_index = 0;
    tiled_texture = frames["down"][0];
    SpriteType = 3;
}

Player::Player(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg)
    : Entity(pos, named_tts, sg)
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
