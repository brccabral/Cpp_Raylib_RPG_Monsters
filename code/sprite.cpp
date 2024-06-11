#include "sprite.h"

#include <iostream>
#include <raymath.h>

Sprite::Sprite(const Vector2 position, Texture2D *image, SpriteGroup *sprite_group, const Rectangle imgRect)
    : position(position), imgRect(imgRect), image(image)
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

AnimatedSprite::AnimatedSprite(
        const Vector2 position, std::vector<Texture2D> &frames, SpriteGroup *sprite_group, const Rectangle imgRect)
    : Sprite(position, &frames[0], sprite_group, imgRect)
{
    std::cout << "animated\n";
    SpriteType = 1;
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
