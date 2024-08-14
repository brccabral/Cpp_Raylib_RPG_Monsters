#include "entities.h"
#include "settings.h"
#include "sprite.h"


Entity::Entity(
        const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
        const std::string &facing_direction)
    : facing_direction(facing_direction), frames_direction(frames)
{
    image = frames[facing_direction];
    image->atlas_rect = frames[facing_direction]->frames[0];
    rect = image->GetRect();
    rect.center(pos);
    z = WORLD_LAYERS["main"];
    y_sort = rect.centery();
    hitbox = rect.inflate(-rect.width / 2, -60);
}

void Entity::Update(const float deltaTime)
{
    Sprite::Update(deltaTime);
    Animate(deltaTime);
}

void Entity::Animate(const float dt)
{
    GetState();
    image = frames_direction[facing_direction];

    frame_index += ANIMATION_SPEED * dt;
    if (frame_index > std::dynamic_pointer_cast<rg::Frames>(image)->frames.size())
    {
        frame_index = 0;
    }
    image->atlas_rect = std::dynamic_pointer_cast<rg::Frames>(image)->frames[int(frame_index)];
}

std::string Entity::GetState()
{
    if (direction.x || direction.y)
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
    else
    {
        if (facing_direction == "down")
        {
            facing_direction = "down_idle";
        }
        if (facing_direction == "up")
        {
            facing_direction = "up_idle";
        }
        if (facing_direction == "left")
        {
            facing_direction = "left_idle";
        }
        if (facing_direction == "right")
        {
            facing_direction = "right_idle";
        }
    }
    return facing_direction;
}

Player::Player(
        const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
        const std::string &facing_direction,
        const std::shared_ptr<rg::sprite::Group> &collision_sprites)
    : Entity(pos, frames, facing_direction), collision_sprites(collision_sprites)
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
    y_sort = rect.centery();
}

void Player::Move(const float dt)
{
    // we split x and y to calculate collisions independently
    rect.x += direction.x * speed * dt;
    hitbox.centerx(rect.centerx());
    Collisions("horizontal");
    rect.y += direction.y * speed * dt;
    hitbox.centery(rect.centery());
    Collisions("vertical");
}

void Player::Collisions(const std::string &axis)
{
    for (const auto &collision_sprite: collision_sprites->Sprites())
    {
        rg::Rect sprite_hitbox;
        if (const auto entity = std::dynamic_pointer_cast<Entity>(collision_sprite))
        {
            sprite_hitbox = entity->hitbox;
        }
        else if (const auto sprite = std::dynamic_pointer_cast<::Sprite>(collision_sprite))
        {
            sprite_hitbox = sprite->hitbox;
        }
        if (hitbox.colliderect(sprite_hitbox))
        {
            if (!strcmp(axis.c_str(), "horizontal"))
            {
                if (direction.x > 0)
                {
                    hitbox.right(sprite_hitbox.left());
                }
                else if (direction.x < 0)
                {
                    hitbox.left(sprite_hitbox.right());
                }
                rect.centerx(hitbox.centerx());
            }
            else // vertical
            {
                if (direction.y > 0)
                {
                    hitbox.bottom(sprite_hitbox.top());
                }
                else if (direction.y < 0)
                {
                    hitbox.top(sprite_hitbox.bottom());
                }
                rect.centery(hitbox.centery());
            }
        }
    }
}

Character::Character(
        const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
        const std::string &facing_direction)
    : Entity(pos, frames, facing_direction)
{}
