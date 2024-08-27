#include <algorithm>
#include "groups.hpp"
#include "settings.hpp"


AllSprites::AllSprites() : Group()
{
    shadow_surf = rg::image::Load("resources/graphics/other/shadow.png");
    notice_surf = rg::image::Load("resources/graphics/ui/notice.png");
}

void AllSprites::Draw(const rg::Surface_Ptr &surface)
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

    // sort main sprites based on `y_sort`
    std::sort(
            main_sprites.begin(), main_sprites.end(),
            [](const std::shared_ptr<rg::sprite::Sprite> &l,
               const std::shared_ptr<rg::sprite::Sprite> &r)
            {
                float yl;
                float yr;
                if (const auto lSprite = std::dynamic_pointer_cast<Sprite>(l))
                {
                    yl = lSprite->y_sort;
                }
                else if (const auto lEntity = std::dynamic_pointer_cast<Entity>(l))
                {
                    yl = lEntity->y_sort;
                }
                else
                {
                    throw;
                }

                if (const auto rSprite = std::dynamic_pointer_cast<Sprite>(r))
                {
                    yr = rSprite->y_sort;
                }
                else if (const auto rEntity = std::dynamic_pointer_cast<Entity>(r))
                {
                    yr = rEntity->y_sort;
                }
                else
                {
                    throw;
                }

                return yl < yr;
            });

    for (const auto &sprite: bg_sprites)
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
    for (const auto &sprite: main_sprites)
    {
        // show shadow below entities
        if (const auto entity = std::dynamic_pointer_cast<Entity>(sprite))
        {
            display_surface->Blit(
                    shadow_surf,
                    sprite->rect.topleft() + offset +
                            rg::math::Vector2{
                                    entity->rect.width / 2.0f -
                                            shadow_surf->GetTexture().width / 2.0f,
                                    entity->rect.height - shadow_surf->GetTexture().height,
                            });
        }
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
        if (sprite == player && player->noticed)
        {
            auto rect = notice_surf->GetRect().midbottom(sprite->rect.midtop());
            display_surface->Blit(notice_surf, rect.topleft() + offset);
        }
    }
    for (const auto &sprite: top_sprites)
    {
        display_surface->Blit(sprite->image, sprite->rect.topleft() + offset);
    }
}

void BattleSprites::Draw(
        const std::shared_ptr<MonsterSprite> &current_monster_sprite, const SelectionSide side,
        const SelectionMode mode, int target_index, const Group *player_sprites,
        const Group *opponent_sprites)
{
    // get available positions
    auto *sprite_group = side == OPPONENT ? opponent_sprites : player_sprites;
    std::shared_ptr<MonsterSprite> monster_sprite = nullptr;
    // when a monster gets defeated, the group may change, but the "pos_index" won't
    // create a list ordered by "pos_index"
    auto ordered_pos = sprite_group->Sprites();
    if (!ordered_pos.empty())
    {
        std::sort(
                ordered_pos.begin(), ordered_pos.end(),
                [](const auto &a, const auto &b)
                {
                    return std::dynamic_pointer_cast<MonsterSprite>(a)->pos_index <
                           std::dynamic_pointer_cast<MonsterSprite>(b)->pos_index;
                });
        target_index = target_index % ordered_pos.size();
        monster_sprite = std::dynamic_pointer_cast<MonsterSprite>(ordered_pos[target_index]);
    }

    auto sprites = Sprites();
    std::sort(
            sprites.begin(), sprites.end(),
            [](const std::shared_ptr<rg::sprite::Sprite> &l,
               const std::shared_ptr<rg::sprite::Sprite> &r) { return l->z < r->z; });
    for (const auto &sprite: sprites)
    {
        if (sprite->z == BATTLE_LAYERS["outline"])
        {
            const auto outline_sprite = std::dynamic_pointer_cast<MonsterOutlineSprite>(sprite);
            if ((outline_sprite->monster_sprite == current_monster_sprite &&
                 !(mode == SELECTMODE_TARGET && side == PLAYER)) ||
                (outline_sprite->monster_sprite == monster_sprite && mode == SELECTMODE_TARGET &&
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
