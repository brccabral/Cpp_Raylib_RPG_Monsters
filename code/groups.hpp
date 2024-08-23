#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "sprite.hpp"


class AllSprites : public rg::sprite::Group
{
public:

    AllSprites();
    void Draw(const std::shared_ptr<rg::Surface> &surface) override;
    void Draw(const std::shared_ptr<Player> &player);

private:

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    rg::math::Vector2 offset{};

    std::shared_ptr<rg::Surface> shadow_surf;
    std::shared_ptr<rg::Surface> notice_surf;
};

class BattleSprites : public rg::sprite::Group
{
public:

    BattleSprites() = default;
    void Draw(const std::shared_ptr<rg::Surface> &surface) override{};
    void
    Draw(const std::shared_ptr<MonsterSprite> &current_monster_sprite, SelectionSide side,
         SelectionMode mode, int target_index, const Group *player_sprites,
         const Group *opponent_sprites);

private:

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
};
