#pragma once
#include <rygame.hpp>
#include "monster.hpp"
#include "settings.hpp"


class MonsterIndex
{
public:

    MonsterIndex() = default;
    MonsterIndex(
            std::map<int, Monster> *monsters,
            std::unordered_map<std::string, rg::font::Font> *fonts,
            std::unordered_map<std::string, rg::Surface> *monster_icons,
            std::unordered_map<std::string, rg::Surface> *ui_icons,
            std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *
            monster_frames);
    void Update(float dt);
    void Input();

private:

    void DisplayList();
    void DisplayMain(float dt);

    rg::Surface *display_surface = &rg::display::GetSurface();
    std::map<int, Monster> *monsters{};
    std::unordered_map<std::string, rg::font::Font> *fonts{};
    rg::Surface tint_surf{};

    rg::Rect main_rect{0.0f, 0.0f, WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.8f};
    int visible_items = 6;
    float list_width = main_rect.width * 0.3f;
    float item_height = main_rect.height / visible_items;

    std::unordered_map<std::string, rg::Surface> *monster_icons{};
    int index{};
    int selected_index = -1;
    std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *monster_frames
            {};
    float frame_index{};
    std::unordered_map<std::string, rg::Surface> *ui_icons{};
    std::unordered_map<std::string, float> max_stats{};

    std::vector<rg::Surface> monster_name_list_surfaces{};
    rg::Surface stats_text_surf{};
    rg::Surface text_surf{};
    rg::Surface ability_text_surf{};
    std::vector<rg::Surface> abilities_surfaces{};
    rg::Surface shadow_surf{};
    rg::Surface name_surf{};
    rg::Surface level_surf{};
    rg::Surface element_surf{};
    rg::Surface hp_text{};
    rg::Surface ep_text{};
    std::vector<rg::Surface> monster_stats_sufaces{};
};
