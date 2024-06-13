#include "sprite.h"
#include "entities.h"
#include <utility>
#include "raylib_utils.h"
#include "settings.h"

#include <cstring>

SimpleSprite::SimpleSprite(const std::vector<SpriteGroup *> &sprite_groups)
{
    for (auto *sprite_group: sprite_groups)
    {
        groups.push_back(sprite_group);
        sprite_group->sprites.push_back(this);
    }
}

void SimpleSprite::Draw(const Vector2 offset) const
{
    const Vector2 pos = Vector2Add({rect.x, rect.y}, offset);

    if (image.texture)
    {
        DrawTextureRec(*image.texture, image.rect, pos, WHITE);
    }
    else
    {
        DrawRectangle(pos.x, pos.y, rect.width, rect.height, BLACK);
    }
}

void SimpleSprite::Update(const double deltaTime)
{}

Sprite::Sprite(const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs, const int z_)
    : SimpleSprite(sgs)
{
    image = img;
    rect = image.rect;
    RectToTopleft(rect, pos);
    z = z_;
    y_sort = GetRectCenter(rect).y;
    type = SPRITE;
    hitbox = rect;
}

MonsterPatchSprite::MonsterPatchSprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs, std::string bio)
    : Sprite(pos, img, sgs, WORLD_LAYERS["main"]), biome(std::move(bio))
{
    y_sort -= 40;
    // move sand patches to background drawing layer
    if (strcmp(biome.c_str(), "sand") == 0)
    {
        z = WORLD_LAYERS["bg"];
    }
}

BorderSprite::BorderSprite(const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, img, sgs)
{}

CollidableSprite::CollidableSprite(const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, img, sgs)
{
    // smaller hitbox for better view perpective on Y axis
    RectInflate(hitbox, 0, -rect.height * 0.6f);
}

AnimatedSprite::AnimatedSprite(
        const Vector2 position, const std::vector<TiledTexture> &frms, const std::vector<SpriteGroup *> &sgs,
        const int z)
    : Sprite(position, frms[0], sgs, z), frame_index(0), frames(frms)
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

int GetYsort(const SimpleSprite *sprite)
{
    int y = 0;

    if (sprite->type == SPRITE)
    {
        y = ((Sprite *) sprite)->y_sort;
    }
    else if (sprite->type == ENTITY)
    {
        y = ((Entity *) sprite)->y_sort;
    }
    return y;
}

Rectangle GetHitbox(const SimpleSprite *sprite)
{
    Rectangle rect{};

    if (sprite->type == SPRITE)
    {
        rect = ((Sprite *) sprite)->hitbox;
    }
    else if (sprite->type == ENTITY)
    {
        rect = ((Entity *) sprite)->hitbox;
    }
    return rect;
}
