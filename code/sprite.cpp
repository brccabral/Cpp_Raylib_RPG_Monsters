#include "sprite.h"

#include <raymath.h>

Sprite::Sprite(const Vector2 position, Texture2D *image, SpriteGroup *sprite_group, const Rectangle imgRect)
    : position(position), image(image), imgRect(imgRect)
{
    groups.push_back(sprite_group);
    sprite_group->sprites.push_back(this);
}

Sprite::~Sprite()
{
    UnloadTexture(*image);
}

void Sprite::Draw(const Vector2 offset) const
{
    DrawTextureRec(*image, imgRect, Vector2Add(position, offset), WHITE);
}

void Sprite::Update(const double deltaTime)
{}

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
