#pragma once
#include <rygame.hpp>
#include "game_data.hpp"
#include "monster.hpp"


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
    bool blocked = false; // block player movements
    rg::math::Vector2 direction{};

protected:

    float speed = 250;

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
           const std::string &facing_direction, rg::sprite::Group *collision_sprites);
    void Update(float dt) override;

    bool noticed = false;

private:

    void Input();
    void Move(float dt);
    void Collisions(const std::string &axis);

    rg::sprite::Group *collision_sprites;
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
            rg::sprite::Group *collision_sprites, float radius, bool nurse,
            const std::shared_ptr<rg::mixer::Sound> &notice_sound);
    void Update(float deltaTime) override;
    [[nodiscard]] std::vector<std::string> GetDialog() const;

    CharacterData *character_data;
    bool can_rotate = true;
    bool nurse;
    std::map<int, std::shared_ptr<Monster>> monsters;

private:

    void Raycast();
    bool HasLineOfSight() const;
    void StartMove();
    void Move(float dt);
    void RandomViewDirection();

    std::shared_ptr<Player> player;
    std::function<void(const std::shared_ptr<Character> &character)> create_dialog;
    rg::sprite::Group *collision_sprites;
    std::vector<rg::Rect> collision_rects;
    bool has_moved{};
    bool has_noticed{};
    float radius{};
    std::vector<std::string> view_directions = {"left", "right"};

    std::map<std::string, rg::Timer> timers;

    std::shared_ptr<rg::mixer::Sound> notice_sound;
};
