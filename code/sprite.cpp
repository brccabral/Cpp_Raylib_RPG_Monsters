#include "sprite.h"
#include "entities.h"
#include <raymath.h>
#include "raylib_utils.h"
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
        DrawTextureRec(*image.texture, image.rect, Vector2Add({rect.x, rect.y}, offset), WHITE);
    }
}

void SimpleSprite::Update(const double deltaTime)
{}

Sprite::Sprite(const Vector2 pos, const TiledTexture &img, SpriteGroup *sg, const int z_) : SimpleSprite(sg)
{
    image = img;
    rect = image.rect;
    RectToTopleft(rect, pos);
    z = z_;
    type = SPRITE;
}

AnimatedSprite::AnimatedSprite(
        const Vector2 position, const std::vector<TiledTexture> &frms, SpriteGroup *sprite_group, const int z)
    : Sprite(position, frms[0], sprite_group, z), frame_index(0), frames(frms)
{}

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
    for (const auto *sprite: sprites)
    {
        sprite->Draw({0, 0});
    }
}

void SpriteGroup::Update(const double deltaTime)
{
    for (auto *sprite: sprites)
    {
        sprite->Update(deltaTime);
    }
}

int GetZ(const SimpleSprite *sprite)
{
    int z = 0;

    if (sprite->type == SPRITE)
    {
        z = ((Sprite *) sprite)->z;
    }
    else if (sprite->type == ENTITY)
    {
        z = ((Entity *) sprite)->z;
    }
    return z;
}
