#pragma once
#include <rygame.h>


class Sprite : public rg::sprite::Sprite
{
public:

    Sprite(rg::math::Vector2 pos, rg::Surface *surf, const std::vector<rg::sprite::Group *> &groups,
           rg::sprite::SpriteOwner *owner);
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(
            rg::math::Vector2 pos, rg::Frames *surf, const std::vector<rg::sprite::Group *> &groups,
            rg::sprite::SpriteOwner *owner);
    ~AnimatedSprite() override;
};
