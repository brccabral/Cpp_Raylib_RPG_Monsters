#pragma once
#include <rygame.hpp>
#include "monster.hpp"
#include "settings.hpp"


class MonsterIndex
{
public:

    MonsterIndex(
            std::map<int, std::shared_ptr<Monster>> *monsters,
            const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts,
            std::map<std::string, rg::Surface_Ptr> *monster_icons,
            std::map<std::string, rg::Surface_Ptr> *ui_icons,
            std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
                    *monster_frames);
    void Update(float dt);
    void Input();

private:

    void DisplayList();
    void DisplayMain(double dt);

    rg::Surface_Ptr display_surface = rg::display::GetSurface();
    std::map<int, std::shared_ptr<Monster>> *monsters;
    std::map<std::string, std::shared_ptr<rg::font::Font>> fonts;
    rg::Surface_Ptr tint_surf;

    rg::Rect main_rect = {0, 0, WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.8f};
    int visible_items = 6;
    float list_width = main_rect.width * 0.3;
    float item_height = main_rect.height / visible_items;

    std::map<std::string, rg::Surface_Ptr> *monster_icons;
    int index{};
    int selected_index = -1;
    std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>> *monster_frames;
    float frame_index{};
    std::map<std::string, rg::Surface_Ptr> *ui_icons;
    std::map<std::string, float> max_stats;
};
