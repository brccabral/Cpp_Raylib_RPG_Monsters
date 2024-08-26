#include <utility>
#include "entities.hpp"
#include "settings.hpp"
#include "sprite.hpp"
#include "support.hpp"


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

void Entity::Block()
{
    blocked = true;
    direction = {};
}

void Entity::Unblock()
{
    blocked = false;
}

void Entity::ChangeFacingDirection(const rg::math::Vector2 target_pos)
{
    const auto relation = target_pos - rect.center();
    if (std::abs(relation.y) < 30)
    {
        if (relation.x > 0)
        {
            facing_direction = "right";
        }
        else
        {
            facing_direction = "left";
        }
    }
    else
    {
        if (relation.y > 0)
        {
            facing_direction = "down";
        }
        else
        {
            facing_direction = "up";
        }
    }
}

void Entity::Animate(const float dt)
{
    image = frames_direction[GetState()];

    frame_index += ANIMATION_SPEED * dt;
    if (frame_index > std::dynamic_pointer_cast<rg::Frames>(image)->frames.size())
    {
        frame_index = 0;
    }
    image->atlas_rect = std::dynamic_pointer_cast<rg::Frames>(image)->frames[int(frame_index)];
}

std::string Entity::GetState()
{
    const bool moving = direction.x || direction.y;
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
    std::string result = facing_direction;
    if (!moving)
    {
        result += "_idle";
    }
    return result;
}

Player::Player(
        const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
        const std::string &facing_direction,
        const std::shared_ptr<rg::sprite::Group> &collision_sprites)
    : Entity(pos, frames, facing_direction), collision_sprites(collision_sprites)
{}

void Player::Input()
{
    if (blocked)
    {
        return;
    }
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
    if (!blocked)
    {
        Input();
        Move(dt);
    }
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
        const std::string &facing_direction, CharacterData *char_data,
        const std::shared_ptr<Player> &player,
        const std::function<void(const std::shared_ptr<Character> &character)> &create_dialog,
        const std::shared_ptr<rg::sprite::Group> &collision_sprites, const float radius,
        const bool nurse)
    : Entity(pos, frames, facing_direction), character_data(char_data), nurse(nurse),
      player(player), create_dialog(create_dialog), radius(radius)
{
    for (const auto &sprite: collision_sprites->Sprites())
    {
        if (sprite.get() != this)
        {
            collision_rects.push_back(sprite->rect);
        }
    }
    view_directions = character_data->directions;
    timers["look around"] = rg::Timer(1.5f, true, true, [this] { RandomViewDirection(); });
    timers["notice"] = rg::Timer{0.5f, false, false, [this] { StartMove(); }};

    for (unsigned int i = 0; i < character_data->monsters.size(); ++i)
    {
        auto [name, level] = character_data->monsters[i];
        monsters[i] = std::make_shared<Monster>(name, level);
    }
}

void Character::Update(const float deltaTime)
{
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }

    Entity::Update(deltaTime);
    // character changes direction, but does not move towards player
    if (character_data->look_around)
    {
        Raycast();
        Move(deltaTime);
    }
}

std::vector<std::string> Character::GetDialog() const
{
    if (character_data->defeated)
    {
        return character_data->dialog.defeated;
    }
    return character_data->dialog.default_;
}

void Character::Raycast()
{
    const auto s = std::dynamic_pointer_cast<Character>(shared_from_this());
    if (!has_moved && !has_noticed && CheckConnections(radius, s, player) && HasLineOfSight())
    {
        player->Block();
        player->ChangeFacingDirection(rect.center());
        timers["notice"].Activate(); // delay character start move
        can_rotate = false; // stop look around
        has_noticed = true;
        player->noticed = true;
    }
}

bool Character::HasLineOfSight() const
{
    if (rect.center().distance_to(player->rect.center()) < radius)
    {
        for (auto collision_rect: collision_rects)
        {
            if (collision_rect.clipline(rect.center(), player->rect.center()))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Character::StartMove()
{
    const auto relation = (player->rect.center() - rect.center()).normalize();
    direction = rg::math::Vector2{std::round(relation.x), std::round(relation.y)};
}

void Character::Move(const float dt)
{
    if (!has_moved && direction)
    {
        if (!hitbox.inflate(10, 10).colliderect(player->hitbox))
        {
            rect.center(rect.center() + direction * speed * dt);
            hitbox.center(rect.center());
        }
        else
        {
            direction = {};
            has_moved = true;
            create_dialog(std::dynamic_pointer_cast<Character>(shared_from_this()));
            player->noticed = false;
        }
    }
}

void Character::RandomViewDirection()
{
    if (can_rotate && view_directions.size() > 1)
    {
        const int randDirection = rl::GetRandomValue(0, view_directions.size() - 1);
        facing_direction = view_directions[randDirection];
    }
}
