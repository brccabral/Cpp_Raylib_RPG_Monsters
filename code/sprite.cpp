#include "sprite.h"

Sprite::Sprite(
        const rg::math::Vector2 pos, rg::Surface surf,
        const std::vector<rg::sprite::Group *> &groups)
    : rg::sprite::Sprite(groups)
{
    image = surf;
    rect = image.GetRect();
    rect.topleft(pos);
}

AnimatedSprite::AnimatedSprite(
        const rg::math::Vector2 pos, rg::Frames frames,
        const std::vector<rg::sprite::Group *> &groups)
    : Sprite(pos, frames, groups)
{}

AnimatedSprite::~AnimatedSprite() = default;
