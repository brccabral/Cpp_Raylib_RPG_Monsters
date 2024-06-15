#include "entities.h"

#include <utility>
#include "raylib_utils.h"
#include "settings.h"
#include "support.h"

#include <iostream>


Entity::Entity(
        const Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
        const std::vector<SpriteGroup *> &sgs, const FacingDirection facing_dir)
    : SimpleSprite(sgs), facing_direction(facing_dir), face_frames(face_frms)
{
    image = face_frames[GetState()][0];
    rect = CreateCenteredRect(image.rect, pos);

    y_sort = GetRectCenter(rect).y;
    type = ENTITY;
    hitbox = rect;
    RectInflate(hitbox, -rect.width / 2.0f, -60.0f);
}

void Entity::Animate(const double dt)
{
    frame_index += ANIMATION_SPEED * dt;
    image = face_frames[GetState()][int(frame_index) % face_frames[GetState()].size()];
}

void Entity::Update(const double dt)
{
    Animate(dt);
}

void Entity::Block()
{
    blocked = true;
    direction = {0, 0};
}

void Entity::Unblock()
{
    blocked = false;
}

void Entity::ChangeFacingDirection(const Vector2 target_pos)
{
    auto [delta_x, delta_y] = Vector2Subtract(target_pos, GetRectCenter(rect));
    if (abs(delta_y) < 30)
    {
        if (delta_x > 0)
        {
            facing_direction = RIGHT_IDLE;
        }
        else
        {
            facing_direction = LEFT_IDLE;
        }
    }
    else
    {
        if (delta_y > 0)
        {
            facing_direction = DOWN_IDLE;
        }
        else
        {
            facing_direction = UP_IDLE;
        }
    }
}

FacingDirection Entity::GetState()
{
    if (direction.x || direction.y)
    {
        if (direction.x)
        {
            facing_direction = direction.x > 0 ? RIGHT : LEFT;
        }
        if (direction.y)
        {
            facing_direction = direction.y > 0 ? DOWN : UP;
        }
    }
    else
    {
        if (facing_direction == DOWN)
        {
            facing_direction = DOWN_IDLE;
        }
        if (facing_direction == UP)
        {
            facing_direction = UP_IDLE;
        }
        if (facing_direction == LEFT)
        {
            facing_direction = LEFT_IDLE;
        }
        if (facing_direction == RIGHT)
        {
            facing_direction = RIGHT_IDLE;
        }
    }
    return facing_direction;
}

Character::Character(
        const Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
        const std::vector<SpriteGroup *> &sgs, const FacingDirection facing_dir, CharacterData char_data,
        Player *player, const SpriteGroup *collision_sprites, const float radius)
    : Entity(pos, face_frms, sgs, facing_dir), character_data(std::move(char_data)), player(player), radius(radius)
{
    for (const auto sprite: collision_sprites->sprites)
    {
        if (sprite != this)
        {
            collition_rects.push_back(sprite->rect);
        }
    }
    view_directions = character_data.directions;
}

std::vector<std::string> Character::GetDialog() const
{
    if (character_data.defeated)
    {
        return character_data.dialog.defeated;
    }
    return character_data.dialog.default_;
}

void Character::Update(const double dt)
{
    Entity::Update(dt);
    Raycast();
    Move(dt);
}

void Character::StartMove()
{
    auto [x, y] = Vector2Normalize(Vector2Subtract(GetRectCenter(player->rect), GetRectCenter(rect)));

    // use round() to avoid diagonal movements
    direction = {std::round(x), std::round(y)};
}

void Character::Move(const double dt)
{
    if (!has_moved && (direction.x || direction.y))
    {
        Rectangle newHitbox = hitbox;
        RectInflate(newHitbox, 10.0f, 10.0f);
        if (!CheckCollisionRecs(newHitbox, player->hitbox))
        {
            MoveRect(rect, Vector2Scale(direction, speed * dt));
            RectToCenter(hitbox, GetRectCenter(rect));
        }
        else
        {
            direction = {};
            has_moved = true;
        }
    }
}

void Character::Raycast()
{
    if (!has_moved && CheckConnections(radius, this, player) && HasLineOfSight())
    {
        player->Block();
        player->ChangeFacingDirection(GetRectCenter(rect));
        StartMove();
    }
}

// There are no collision object (rectangle) between Character and Player
bool Character::HasLineOfSight()
{
    const Vector2 center = GetRectCenter(rect);
    const Vector2 centerplayer = GetRectCenter(player->rect);

    if (Vector2Distance(center, centerplayer) < radius)
    {
        for (const auto rect: collition_rects)
        {
            Vector2 collision1{};
            Vector2 collision2{};
            if (CheckCollisionRectLine(rect, center, centerplayer, &collision1, &collision2))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

Player::Player(
        const Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
        const std::vector<SpriteGroup *> &sgs, const FacingDirection facing_dir, SpriteGroup *cs)
    : Entity(pos, face_frms, sgs, facing_dir), collision_sprites(cs)
{}

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
    direction = Vector2Normalize(input_vector);
}

void Player::Move(const double deltaTime)
{
    rect.x += direction.x * speed * deltaTime;
    RectToCenter(hitbox, GetRectCenter(rect));
    Collisions(HORIZONTAL);

    rect.y += direction.y * speed * deltaTime;
    RectToCenter(hitbox, GetRectCenter(rect));
    Collisions(VERTICAL);
}

void Player::Update(const double deltaTime)
{
    Entity::Update(deltaTime);
    y_sort = GetRectCenter(rect).y;
    if (!blocked)
    {
        Input();
        Move(deltaTime);
    }
}

void Player::Collisions(const Axis axis)
{
    for (const auto *sprite: collision_sprites->sprites)
    {
        const Rectangle sprite_hitbox = GetHitbox(sprite);
        if (CheckCollisionRecs(sprite_hitbox, hitbox))
        {
            if (axis == HORIZONTAL)
            {
                if (direction.x > 0)
                {
                    hitbox.x = sprite_hitbox.x - hitbox.width;
                }
                else if (direction.x < 0)
                {
                    hitbox.x = sprite_hitbox.x + sprite_hitbox.width;
                }
            }
            else
            {
                if (direction.y > 0)
                {
                    hitbox.y = sprite_hitbox.y - hitbox.height;
                }
                else if (direction.y < 0)
                {
                    hitbox.y = sprite_hitbox.y + sprite_hitbox.height;
                }
            }
            RectToCenter(rect, GetRectCenter(hitbox));
        }
    }
}

Vector2 Player::GetCenter() const
{
    return GetRectCenter(rect);
}
