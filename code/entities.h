#pragma once
#include <rygame.h>


class Player : public rg::sprite::Sprite
{
public:

    explicit Player(rg::math::Vector2 pos);
    void Update(float dt) override;

protected:

    rg::math::Vector2 direction{};
    float speed = 250;

private:

    void Input();
    void Move(float dt);
};
