#include "sprite.h"

#include "settings.h"

Sprite::Sprite(const rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf)
{
    image = surf;
    rect = image->GetRect();
    rect.topleft(pos);
}

AnimatedSprite::AnimatedSprite(const rg::math::Vector2 pos, const std::shared_ptr<rg::Frames> &surf)
    : Sprite(pos, surf), image(surf)
{}

void AnimatedSprite::Update(const float deltaTime)
{
    Animate(deltaTime);
}

void AnimatedSprite::Animate(const float dt)
{
    frame_index += ANIMATION_SPEED * dt;
    if (frame_index > image->frames.size())
    {
        frame_index = 0;
    }
    image->SetAtlas(int(frame_index));
}
