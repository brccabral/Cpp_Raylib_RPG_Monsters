#pragma once
#include <vector>
#include <raylib.h>
#include "monster.h"
#include "settings.h"


class MonsterIndex
{
public:

    MonsterIndex(const std::vector<Monster> &monsters, const std::map<std::string, Font> &fonts);
    void Update(double dt);

private:

    void DisplayList();

    RenderTexture2D tint_surface{};
    std::map<std::string, Font> fonts;
    Rectangle main_rect{0, 0, WINDOW_WIDTH * 0.6, WINDOW_HEIGHT * 0.8};
    std::vector<Monster> monsters;
    int visible_items = 6;
    float list_width = main_rect.width * 0.3;
    float item_height = main_rect.height / visible_items;
};
