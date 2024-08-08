#pragma once
#include <rygame.h>
#include "entities.h"


class AllSprites : public rg::sprite::Group
{
public:

    AllSprites();
    void Draw(rg::Surface &surface) override;
    void Draw(const Player *player);

private:

    rg::Surface *display_surface = rg::display::GetSurface();
    rg::math::Vector2 offset{};
};
