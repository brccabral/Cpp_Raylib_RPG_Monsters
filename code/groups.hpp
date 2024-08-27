#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "sprite.hpp"


class AllSprites : public rg::sprite::Group
{
public:

    AllSprites();
    void Draw(const rg::Surface_Ptr &surface) override;
    void Draw(const std::shared_ptr<Player> &player);

private:

    rg::Surface_Ptr display_surface = rg::display::GetSurface();
    rg::math::Vector2 offset{};

    rg::Surface_Ptr shadow_surf;
    rg::Surface_Ptr notice_surf;
};

class BattleSprites : public rg::sprite::Group
{
public:

    BattleSprites() = default;
    void Draw(const rg::Surface_Ptr &surface) override{};
    void
    Draw(const std::shared_ptr<MonsterSprite> &current_monster_sprite, SelectionSide side,
         SelectionMode mode, int target_index, const Group *player_sprites,
         const Group *opponent_sprites);

private:

    rg::Surface_Ptr display_surface = rg::display::GetSurface();
};
