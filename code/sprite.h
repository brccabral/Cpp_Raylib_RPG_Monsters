#pragma once
#include <rygame.h>


class Sprite : public rg::sprite::Sprite
{
public:

    Sprite(rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf);
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(rg::math::Vector2 pos, const std::shared_ptr<rg::Frames> &surf);

    void Update(float deltaTime) override;
    void Animate(float dt);

    std::shared_ptr<rg::Frames> image;

private:

    float frame_index{};
};
