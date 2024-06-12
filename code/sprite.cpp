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
    if (image)
    {
        DrawTextureRec(*image, imgRect, Vector2Add(position, offset), WHITE);
    }
}

void SimpleSprite::Update(const double deltaTime)
{}

SimpleSprite::~SimpleSprite()
{
    if (image)
    {
        UnloadTexture(*image);
    }
}

Sprite::Sprite(const Vector2 pos, Texture2D *im, SpriteGroup *sg, const Rectangle rect) : SimpleSprite(sg)
{
    position = pos;
    imgRect = rect;
    image = im;
    SpriteType = 4;
}

AnimatedSprite::AnimatedSprite(
        const Vector2 position, std::vector<Texture2D> &frames, SpriteGroup *sprite_group, const Rectangle imgRect)
    : Sprite(position, &frames[0], sprite_group, imgRect), frame_index(0), frames(frames)
{
    SpriteType = 1;
}

void AnimatedSprite::Animate(const double deltaTime)
{
    frame_index += ANIMATION_SPEED * deltaTime;
    image = &frames[int(frame_index) % frames.size()];
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
