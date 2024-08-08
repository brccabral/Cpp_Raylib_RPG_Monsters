#include "sprite.h"

Sprite::Sprite(const rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf)
{
    image = surf;
    rect = image->GetRect();
    rect.topleft(pos);
}

AnimatedSprite::AnimatedSprite(const rg::math::Vector2 pos, const std::shared_ptr<rg::Frames> &surf)
    : Sprite(pos, surf)
{}
