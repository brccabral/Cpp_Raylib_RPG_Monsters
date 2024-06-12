#include "sprite.h"
#include <raymath.h>
#include "settings.h"

SimpleSprite::SimpleSprite(SpriteGroup *sprite_group)
{
    if (sprite_group)
    {
        groups.push_back(sprite_group);
        sprite_group->sprites.push_back(this);
    }
}

void SimpleSprite::Draw(const Vector2 offset) const
{
    if (image.texture)
    {
        DrawTextureRec(*image.texture, image.rect, Vector2Add(position, offset), WHITE);
    }
}

void SimpleSprite::Update(const double deltaTime)
{}

Sprite::Sprite(const Vector2 pos, const TiledTexture &img, SpriteGroup *sg, const int z_) : SimpleSprite(sg)
{
    position = pos;
    image = img;
    z = z_;
    SpriteType = 4;
}

AnimatedSprite::AnimatedSprite(
        const Vector2 position, const std::vector<TiledTexture> &frms, SpriteGroup *sprite_group, const int z)
    : Sprite(position, frms[0], sprite_group, z), frame_index(0), frames(frms)
{
    SpriteType = 1;
}

void AnimatedSprite::Animate(const double deltaTime)
{
    frame_index += ANIMATION_SPEED * deltaTime;
    image = frames[int(frame_index) % frames.size()];
}

void AnimatedSprite::Update(const double deltaTime)
{
    Animate(deltaTime);
}

void SpriteGroup::Draw() const
{
    for (const auto &sprite: sprites)
    {
        sprite->Draw({0, 0});
    }
}

void SpriteGroup::Update(const double deltaTime)
{
    for (const auto &sprite: sprites)
    {
        sprite->Update(deltaTime);
    }
}
