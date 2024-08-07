#include "sprite.h"

Sprite::Sprite(
        const rg::math::Vector2 pos, rg::Surface *surf,
        const std::vector<rg::sprite::Group *> &groups, rg::sprite::SpriteOwner *owner)
    : rg::sprite::Sprite(groups, owner)
{
    image = surf;
    rect = image->GetRect();
    rect.topleft(pos);
}

AnimatedSprite::AnimatedSprite(
        const rg::math::Vector2 pos, rg::Frames *surf,
        const std::vector<rg::sprite::Group *> &groups, rg::sprite::SpriteOwner *owner)
    : Sprite(pos, surf, groups, owner)
{}

AnimatedSprite::~AnimatedSprite()
{
    // image is deleted in ~Game()
    image = nullptr;
};
