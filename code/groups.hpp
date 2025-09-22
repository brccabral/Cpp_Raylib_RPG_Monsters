#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "sprite.hpp"


class AllSprites : public rg::sprite::Group
{
public:

    AllSprites();
    void Draw(rg::Surface *surface) override;
    void Draw(Player *player);

private:

    rg::Surface *display_surface = &rg::display::GetSurface();
    rg::math::Vector2 offset{};

    rg::Surface shadow_surf;
    rg::Surface notice_surf;
};

class BattleSprites : public rg::sprite::Group
{
public:

    BattleSprites() = default;

    void Draw(rg::Surface *surface) override
    {
    };

    void Draw(
            const MonsterSprite *current_monster_sprite, SelectionSide side,
            SelectionMode mode, int target_index, const Group *player_sprites,
            const Group *opponent_sprites);

private:

    rg::Surface *display_surface = &rg::display::GetSurface();
};
