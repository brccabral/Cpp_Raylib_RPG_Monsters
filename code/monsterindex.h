#pragma once
#include <vector>
#include <raylib.h>
#include "monster.h"
#include "settings.h"
#include "sprite.h"
#include "support.h"


class MonsterIndex
{
public:

    MonsterIndex(
            const std::vector<Monster> &monsters, const std::map<std::string, Font> &fonts,
            const std::map<std::string, Texture2D> &monster_icons,
            const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                    &monsters_frms,
            const std::map<std::string, Texture2D> &ui_frms);
    void Update(double dt);
    static std::map<std::string, float> GetMaxStats();

private:

    void DisplayList();
    void DisplayMain(double dt);
    void Input();

    RenderTexture2D tint_surface{};
    std::map<std::string, Font> fonts;
    RectangleU main_rect{0, 0, WINDOW_WIDTH * 0.6, WINDOW_HEIGHT * 0.8};
    std::vector<Monster> monsters;
    std::map<std::string, float> max_stats = GetMaxStats();

    // frames
    std::map<std::string, Texture2D> icon_frames;
    std::map<std::string, Texture2D> ui_frames;
    std::map<std::string, std::map<std::string, std::vector<TiledTexture>>> monsters_frames;
    double frame_index{};

    // list
    int visible_items = 6;
    float list_width = main_rect.width * 0.3;
    float item_height = main_rect.height / visible_items;
    int index = 0;
    int selected_index = -1;
};
