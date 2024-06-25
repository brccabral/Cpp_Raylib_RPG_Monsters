#include <algorithm>
#include "groups.h"

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
            RectangleU rect = {0, 0, float(notice.width), float(notice.height)};
            RectToMidBottom(rect, GetRectMidTop(player->rect));

            const Vector2 pos = Vector2Add(rect.pos, offset);

            DrawTextureV(notice, pos, WHITE);
        }
    }
    for (const auto *top: top_sprites)
    {
        top->Draw(offset);
    }
}

void BattleSprites::Draw(
        const MonsterSprite *current_monster_sprite, const SelectionSide side,
        const SelectionMode mode, const int target_index, SpriteGroup *player_sprites,
        SpriteGroup *opponent_sprites)
{
    // get available positions
    SpriteGroup *sprite_group = side == OPPONENT ? opponent_sprites : player_sprites;
    // when a monster gets defeated, the group may change, but the "pos_index" won't
    // create a list with just the "pos_index"
    std::vector<int> sprites_indexes;
    for (auto *sprite: sprite_group->sprites)
    {
        sprites_indexes.push_back(((MonsterSprite *) sprite)->pos_index);
    }

    // TODO : when we remove monsters from group with Kill, these indexes should change.
    auto *monster_sprite = (MonsterSprite *) sprite_group->sprites[sprites_indexes[target_index]];

    std::sort(
            sprites.begin(), sprites.end(),
            [](const SimpleSprite *l, const SimpleSprite *r)
            {
                const int yl = GetYsort(l);
                const int yr = GetYsort(r);

                return yl < yr;
            });
    for (const auto *sprite: sprites)
    {
        if (GetYsort(sprite) == BATTLE_LAYERS["outline"])
        {
            if ((((MonsterOutlineSprite *) sprite)->monster_sprite == current_monster_sprite &&
                 !(mode == SELECTMODE_TARGET && side == PLAYER)) ||
                (((MonsterOutlineSprite *) sprite)->monster_sprite == monster_sprite) &&
                        monster_sprite->entity == side && mode == SELECTMODE_TARGET)
            {
                ((MonsterOutlineSprite *) sprite)->Draw({0, 0});
            }
        }
        else
        {
            DrawTextureRec(
                    *sprite->image.texture, sprite->image.rect.rectangle, sprite->rect.pos, WHITE);
        }
    }
}
