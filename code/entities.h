#pragma once
#include <rygame.h>


class Entity : public rg::sprite::Sprite
{
public:

    Entity(const rg::math::Vector2 &pos, std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
           const std::string &facing_direction);
    void Update(float deltaTime) override;

    float y_sort;
    rg::Rect hitbox;
    std::string facing_direction;

protected:

    rg::math::Vector2 direction{};
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
            const std::string &facing_direction);
};
