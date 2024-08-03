#pragma once
#include <rygame.h>


class Sprite : public rg::sprite::Sprite
{
public:

    Sprite(rg::math::Vector2 pos, rg::Surface surf, const std::vector<rg::sprite::Group *> &groups);
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(
            rg::math::Vector2 pos, rg::Frames frames,
            const std::vector<rg::sprite::Group *> &groups);
    ~AnimatedSprite() override;
};
