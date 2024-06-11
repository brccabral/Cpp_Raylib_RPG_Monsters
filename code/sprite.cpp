#include "sprite.h"

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

void Sprite::Draw() const
{
    DrawTextureRec(*image, imgRect, position, WHITE);
}

void SpriteGroup::Draw() const
{
    for (const auto &sprite: sprites)
    {
        sprite->Draw();
    }
}
