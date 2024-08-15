#pragma once
#include <rygame.h>
#include "monster.h"


class MonsterIndex
{
public:

    MonsterIndex(
            std::map<int, Monster> *monsters,
            const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts);
    void Update(float dt);

private:

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    // std::map<int, Monster> *monsters;
    std::map<std::string, std::shared_ptr<rg::font::Font>> fonts;
    std::shared_ptr<rg::Surface> tint_surf;
};
