#pragma once
#include <rygame.h>
#include "monster.h"


class MonsterIndex
{
public:

    MonsterIndex(
            std::map<int, Monster> *monsters,
            const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts,
            std::map<std::string, std::shared_ptr<rg::Surface>> *monster_icons);
    void Update(float dt);
    void Input();

private:

    void DisplayList();
    void DisplayMain(double dt);

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    std::map<int, Monster> *monsters;
    std::map<std::string, std::shared_ptr<rg::font::Font>> fonts;
    std::shared_ptr<rg::Surface> tint_surf;

    rg::Rect main_rect = {0, 0, WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.8f};
    int visible_items = 6;
    float list_width = main_rect.width * 0.3;
    float item_height = main_rect.height / visible_items;

    std::map<std::string, std::shared_ptr<rg::Surface>> *monster_icons;
    int index{};
    int selected_index = -1;
};
