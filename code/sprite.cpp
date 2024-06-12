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
    if (tiled_texture.texture)
    {
        // DrawTextureV(*tiled_texture.texture, Vector2Add(position, offset), WHITE);
        DrawTextureRec(*tiled_texture.texture, tiled_texture.rect, Vector2Add(position, offset), WHITE);
    }
}

void SimpleSprite::Update(const double deltaTime)
{}

Sprite::Sprite(const Vector2 pos, Texture2D *tx, Rectangle rect, SpriteGroup *sg) : SimpleSprite(sg)
{
    position = pos;
    tiled_texture.texture = tx;
    tiled_texture.rect = rect;
    SpriteType = 4;
}

AnimatedSprite::AnimatedSprite(
        const Vector2 position, Texture2D *tx, const std::vector<Rectangle> &rects, SpriteGroup *sprite_group)
    : Sprite(position, tx, rects[0], sprite_group), frame_index(0), frames(rects)
{
    SpriteType = 1;
}

void AnimatedSprite::Animate(const double deltaTime)
{
    frame_index += ANIMATION_SPEED * deltaTime;
    tiled_texture.rect = frames[int(frame_index) % frames.size()];
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
