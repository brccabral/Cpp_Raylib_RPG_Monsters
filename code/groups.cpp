#include <algorithm>
#include "groups.h"
#include "settings.h"
#include "raylib_utils.h"

AllSprites::AllSprites()
{
    shadow = LoadTexture("resources/graphics/other/shadow.png");
    notice = LoadTexture("resources/graphics/ui/notice.png");
}

AllSprites::~AllSprites()
{
    UnloadTexture(shadow);
    UnloadTexture(notice);
}

void AllSprites::Draw(const Player *player)
{
    offset = ViewOffset(player->GetCenter());

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
                const int yl = GetYsort(l);
                const int yr = GetYsort(r);

                return yl < yr;
            });
    for (const auto *bg: bg_sprites)
    {
        bg->Draw(offset);
    }
    for (const auto *main: main_sprites)
    {
        if (main->type == ENTITY)
        {
            DrawTextureV(
                    shadow,
                    Vector2Add(
                            {main->rect.x + main->rect.width / 2.0f - shadow.width / 2.0f,
                             main->rect.y + main->rect.height - shadow.height},
                            offset),
                    WHITE);
        }
        main->Draw(offset);
        if (main == player && player->noticed)
        {
            Rectangle rect = {0, 0, float(notice.width), float(notice.height)};
            RectToMidBottom(rect, GetRectMidTop(player->rect));

            const Vector2 pos = Vector2Add({rect.x, rect.y}, offset);

            DrawTexture(notice, pos.x, pos.y, WHITE);
        }
    }
    for (const auto *top: top_sprites)
    {
        top->Draw(offset);
    }
}
