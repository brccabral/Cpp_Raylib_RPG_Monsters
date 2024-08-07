#pragma once
#include <raylib.h>
#include <vector>
#include "monster.h"
#include "sprite.h"


class MonsterIndex
{
public:

    MonsterIndex(
            std::map<int, Monster> *monsters, const std::map<std::string, Font> &fonts,
            const std::map<std::string, Texture2D> &monster_icons,
            const std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>>
                    &monsters_frms,
            const std::map<std::string, Texture2D> &ui_frms);
    ~MonsterIndex();
    void Update(double dt);
    static std::map<std::string, float> GetMaxStats();

private:

    void DisplayList();
    void DisplayMain(double dt);
    void Input();

    RenderTexture2D tint_surface{};
    std::map<std::string, Font> fonts;
    RectangleU main_rect{0, 0, WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.8f};
    std::map<int, Monster> *monsters;
    std::map<std::string, float> max_stats = GetMaxStats();

    // frames
    std::map<std::string, Texture2D> icon_frames;
    std::map<std::string, Texture2D> ui_frames;
    std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>> monsters_frames;
    double frame_index{};

    // list
    int visible_items = 6;
    float list_width = main_rect.width * 0.3f;
    float item_height = main_rect.height / visible_items;
    int index = 0;
    int selected_index = -1;
};
