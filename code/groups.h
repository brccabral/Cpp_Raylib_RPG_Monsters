#pragma once
#include <rygame.h>
#include "entities.h"


class AllSprites : public rg::sprite::Group
{
public:

    AllSprites();
    void Draw(const std::shared_ptr<rg::Surface> &surface) override;
    void Draw(const std::shared_ptr<Player> &player);

private:

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    rg::math::Vector2 offset{};
};
