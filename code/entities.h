#pragma once
#include <rygame.h>


class Entity : public rg::sprite::Sprite
{
public:

    Entity(const rg::math::Vector2 &pos,
           std::map<std::string, std::shared_ptr<rg::Frames>> &frames);

private:

    std::map<std::string, std::shared_ptr<rg::Frames>> frames_direction;
};

class Player : public Entity
{
public:

    Player(const rg::math::Vector2 &pos,
           std::map<std::string, std::shared_ptr<rg::Frames>> &frames);
    void Update(float dt) override;

protected:

    rg::math::Vector2 direction{};
    float speed = 250;

private:

    void Input();
    void Move(float dt);
};
