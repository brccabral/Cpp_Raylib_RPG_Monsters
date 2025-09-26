#include <algorithm>
#include "groups.hpp"
#include "settings.hpp"


AllSprites::AllSprites() : Group()
{
    shadow_surf = rg::image::Load("resources/graphics/other/shadow.png");
    notice_surf = rg::image::Load("resources/graphics/ui/notice.png");
}

void AllSprites::Draw(rg::Surface *surface)
{
    Group::Draw(surface);
}

void AllSprites::Draw(const Player *player)
{
    offset = player->rect.center() - rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    offset *= -1.0f;

    const auto &current_sprites = Sprites();

    bg_sprites.clear();
    bg_sprites.reserve(current_sprites.size());
    main_sprites.clear();
    main_sprites.reserve(current_sprites.size());
    top_sprites.clear();
    top_sprites.reserve(current_sprites.size());

    for (auto *sprite: current_sprites)
    {
        const int z = sprite->z;
        if (z < Settings::GetInstance().WORLD_LAYERS["main"])
        {
            bg_sprites.push_back(sprite);
        }
        else if (z == Settings::GetInstance().WORLD_LAYERS["main"])
        {
            main_sprites.push_back(sprite);
        }
        else if (z > Settings::GetInstance().WORLD_LAYERS["main"])
        {
            top_sprites.push_back(sprite);
        }
    }

    // sort main sprites based on `y_sort`
    std::ranges::sort(
            main_sprites,
            [](rg::sprite::Sprite *l, rg::sprite::Sprite *r)
            {
                float yl;
                float yr;
                if (const auto lSprite = dynamic_cast<Sprite *>(l))
                {
                    yl = lSprite->y_sort;
                }
                else if (const auto lEntity = dynamic_cast<Entity *>(l))
                {
                    yl = lEntity->y_sort;
                }
                else
                {
                    throw;
                }

                if (const auto rSprite = dynamic_cast<Sprite *>(r))
                {
                    yr = rSprite->y_sort;
                }
                else if (const auto rEntity = dynamic_cast<Entity *>(r))
                {
                    yr = rEntity->y_sort;
                }
                else
                {
                    throw;
                }

                return yl < yr;
            });

    for (const auto *sprite: bg_sprites)
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
    for (auto *sprite: main_sprites)
    {
        // show shadow below entities
        if (const auto entity = dynamic_cast<Entity *>(sprite))
        {
            display_surface->Blit(
                    &shadow_surf,
                    sprite->rect.topleft() + offset +
                    rg::math::Vector2{
                            entity->rect.width / 2.0f -
                            shadow_surf.GetTexture().width / 2.0f,
                            entity->rect.height - shadow_surf.GetTexture().height,
                    });
        }
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
        if (player == sprite && player->noticed)
        {
            auto rect = notice_surf.GetRect().midbottom(sprite->rect.midtop());
            display_surface->Blit(&notice_surf, rect.topleft() + offset);
        }
    }
    for (const auto &sprite: top_sprites)
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
}

void BattleSprites::Draw(
        const MonsterSprite *current_monster_sprite, const SelectionSide side,
        const SelectionMode mode, int target_index, Group *player_sprites,
        Group *opponent_sprites)
{
    // get available positions
    auto *sprite_group = side == OPPONENT ? opponent_sprites : player_sprites;
    const MonsterSprite *monster_sprite = nullptr;
    // when a monster gets defeated, the group may change, but the "pos_index" won't
    // create a list ordered by "pos_index"
    auto ordered_pos = sprite_group->Sprites(); // force a copy
    if (!ordered_pos.empty())
    {
        std::ranges::sort(
                ordered_pos,
                [](const auto &a, const auto &b)
                {
                    return dynamic_cast<MonsterSprite *>(a)->pos_index <
                           dynamic_cast<MonsterSprite *>(b)->pos_index;
                });
        target_index = target_index % ordered_pos.size();
        monster_sprite = dynamic_cast<MonsterSprite *>(ordered_pos[target_index]);
    }

    auto sprites_ = Sprites(); // force a copy
    std::ranges::sort(
            sprites_,
            [](const rg::sprite::Sprite *l, const rg::sprite::Sprite *r)
            {
                return l->z < r->z;
            });
    for (auto *sprite: sprites_)
    {
        if (sprite->z == Settings::GetInstance().BATTLE_LAYERS["outline"])
        {
            const auto outline_sprite = dynamic_cast<MonsterOutlineSprite *>(sprite);
            if ((outline_sprite->monster_sprite == current_monster_sprite &&
                 !(mode == SELECTMODE_TARGET && side == PLAYER)) ||
                (monster_sprite && outline_sprite->monster_sprite == monster_sprite && mode ==
                 SELECTMODE_TARGET &&
                 side == monster_sprite->entity))
            {
                display_surface->Blit(outline_sprite->image, outline_sprite->rect);
            }
        }
        else
        {
            display_surface->Blit(sprite->image, sprite->rect);
        }
    }
}
