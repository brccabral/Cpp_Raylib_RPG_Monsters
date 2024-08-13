#include "groups.h"

#include "settings.h"


AllSprites::AllSprites() : Group()
{}

void AllSprites::Draw(const std::shared_ptr<rg::Surface> &surface)
{
    Group::Draw(surface);
}

void AllSprites::Draw(const std::shared_ptr<Player> &player)
{
    offset = player->rect.center() - rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    offset *= -1.0f;

    auto sprites = Sprites();
    std::vector<std::shared_ptr<rg::sprite::Sprite>> bg_sprites;
    bg_sprites.reserve(sprites.size());
    std::vector<std::shared_ptr<rg::sprite::Sprite>> main_sprites;
    main_sprites.reserve(sprites.size());
    std::vector<std::shared_ptr<rg::sprite::Sprite>> top_sprites;
    top_sprites.reserve(sprites.size());
    for (const auto &sprite: sprites)
    {
        const int z = sprite->z;
        if (z < WORLD_LAYERS["main"])
        {
            bg_sprites.push_back(sprite);
        }
        else if (z == WORLD_LAYERS["main"])
        {
            main_sprites.push_back(sprite);
        }
        else if (z > WORLD_LAYERS["main"])
        {
            top_sprites.push_back(sprite);
        }
    }

    for (const auto &sprite: bg_sprites)
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
    for (const auto &sprite: main_sprites)
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
    for (const auto &sprite: top_sprites)
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
}
