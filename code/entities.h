#pragma once
#include <rygame.h>


class Entity : public rg::sprite::Sprite
{
public:

    Entity(const rg::math::Vector2 &pos,
           std::map<std::string, std::shared_ptr<rg::Frames>> &frames);
    void Update(float deltaTime) override;

protected:

    rg::math::Vector2 direction{};
    float speed = 250;
    std::string facing_direction = "down_idle";

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

    Player(const rg::math::Vector2 &pos,
           std::map<std::string, std::shared_ptr<rg::Frames>> &frames);
    void Update(float dt) override;

private:

    void Input();
    void Move(float dt);
};

class Character : public Entity
{
public:

    Character(
            const rg::math::Vector2 &pos,
            std::map<std::string, std::shared_ptr<rg::Frames>> &frames);
};
