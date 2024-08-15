#pragma once
#include "game_data.h"


#include <rygame.h>


class Entity : public rg::sprite::Sprite
{
public:

    Entity(const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
           const std::string &facing_direction);
    void Update(float deltaTime) override;
    void Block();
    void Unblock();
    void ChangeFacingDirection(rg::math::Vector2 target_pos);

    float y_sort;
    rg::Rect hitbox{};
    std::string facing_direction;

protected:

    rg::math::Vector2 direction{};
    float speed = 250;
    bool blocked = false;

private:

    // update state and change animation image
    void Animate(float dt);
    // update and return state
    std::string GetState();

    std::map<std::string, std::shared_ptr<rg::Frames>> frames_direction;
    float frame_index{};
};

class Player : public Entity
{
public:

    Player(const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
           const std::string &facing_direction,
           const std::shared_ptr<rg::sprite::Group> &collision_sprites);
    void Update(float dt) override;

private:

    void Input();
    void Move(float dt);
    void Collisions(const std::string &axis);

    std::shared_ptr<rg::sprite::Group> collision_sprites;
};

class Character : public Entity
{
public:

    Character(
            const rg::math::Vector2 &pos,
            std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
            const std::string &facing_direction, CharacterData *char_data,
            const std::shared_ptr<Player> &player,
            const std::function<void(const std::shared_ptr<Character> &character)> &create_dialog,
            const std::shared_ptr<rg::sprite::Group> &collision_sprites, float radius);
    void Update(float deltaTime) override;
    [[nodiscard]] std::vector<std::string> GetDialog() const;

    CharacterData *character_data;
    bool can_rotate = true;

private:

    void Raycast();
    bool HasLineOfSight() const;

    std::shared_ptr<Player> player;
    std::function<void(const std::shared_ptr<Character> &character)> create_dialog;
    std::shared_ptr<rg::sprite::Group> collision_sprites;
    std::vector<rg::Rect> collision_rects;
    bool has_moved{};
    bool has_noticed{};
    float radius{};
    std::vector<std::string> view_directions = {"left", "right"};
};
