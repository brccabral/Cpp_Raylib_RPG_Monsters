#include "groups.h"

#include "settings.h"


AllSprites::AllSprites() : Group()
{}

void AllSprites::Draw(const Player *player)
{
    offset = player->rect.center() - rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    offset *= -1.0f;

    for (const auto *sprite: Sprites())
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
}
