#include "groups.h"
#include "entities.h"
#include <raymath.h>
#include "settings.h"

#include <algorithm>

void AllSprites::Draw(const Vector2 player_center)
{
    offset = Vector2Subtract(player_center, Vector2Scale({WINDOW_WIDTH, WINDOW_HEIGHT}, 0.5));
    offset = Vector2Scale(offset, -1);

    std::vector<SimpleSprite *> bg_sprites;
    std::vector<SimpleSprite *> main_sprites;
    std::vector<SimpleSprite *> top_sprites;

    for (auto *sprite: sprites)
    {
        const int z = GetZ(sprite);
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
    std::sort(
            main_sprites.begin(), main_sprites.end(),
            [](const SimpleSprite *l, const SimpleSprite *r)
            {
                const int zl = GetZ(l);
                const int zr = GetZ(r);

                return zl < zr;
            });
    for (const auto *bg: bg_sprites)
    {
        bg->Draw(offset);
    }
    for (const auto *main: main_sprites)
    {
        main->Draw(offset);
    }
    for (const auto *top: top_sprites)
    {
        top->Draw(offset);
    }
}
