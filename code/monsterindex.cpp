#include "monsterindex.h"

#include <raylib.h>


MonsterIndex::MonsterIndex(
        std::map<int, Monster> *monsters,
        const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts,
        std::map<std::string, std::shared_ptr<rg::Surface>> *monster_icons)
    : monsters(monsters), fonts(fonts), monster_icons(monster_icons)
{
    tint_surf = std::make_shared<rg::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT);
    tint_surf->SetAlpha(200);
    main_rect.center(rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
}

void MonsterIndex::Update(float dt)
{
    // input
    Input();
    // tint main game
    display_surface->Blit(tint_surf, {0, 0});
    // display the list
    DisplayList();
    // display the main section
}

void MonsterIndex::Input()
{
    if (IsKeyPressed(rl::KEY_UP))
    {
        index -= 1;
    }
    if (IsKeyPressed(rl::KEY_DOWN))
    {
        index += 1;
    }
    const int size = (int) monsters->size();
    index = (index % size + size) % size;

    if (IsKeyPressed(rl::KEY_SPACE))
    {
        // one SPACE to select, another to change order
        if (selected_index != -1)
        {
            // second SPACE is to change the order
            const auto selected_monster = (*monsters)[selected_index];
            const auto current_monster = (*monsters)[index];
            (*monsters)[index] = selected_monster;
            (*monsters)[selected_index] = current_monster;
            selected_index = -1;
        }
        else
        {
            // first SPACE is to select the monster from the list
            selected_index = index;
        }
    }
}

void MonsterIndex::DisplayList()
{
    float v_offset = (index < visible_items) ? 0 : -(index - visible_items + 1) * item_height;
    for (const auto &[list_index, monster]: *monsters)
    {
        // colors
        auto bg_color = (index != list_index) ? COLORS["gray"] : COLORS["light"];
        auto text_color = (selected_index != list_index) ? COLORS["white"] : COLORS["gold"];

        const float top = main_rect.top() + list_index * item_height + v_offset;
        const rg::Rect item_rect = {main_rect.x, top, list_width, item_height};

        auto text_surf = fonts["regular"]->render(monster.name.c_str(), text_color);
        auto text_rect =
                text_surf->GetRect().midleft(item_rect.midleft() + rg::math::Vector2{90, 0});

        auto icon_surf = (*monster_icons)[monster.name];
        auto icon_rect =
                icon_surf->GetRect().center(item_rect.midleft() + rg::math::Vector2{45, 0});

        if (item_rect.colliderect(main_rect))
        {
            // check corners
            if (item_rect.collidepoint(main_rect.topleft()))
            {
                rg::draw::rect(
                        display_surface, bg_color, item_rect, 0, 12, true, false, false, false);
            }
            else if (item_rect.collidepoint(main_rect.bottomleft() + rg::math::Vector2{1, -1}))
            {
                rg::draw::rect(
                        display_surface, bg_color, item_rect, 0, 12, false, false, true, false);
            }
            else
            {
                rg::draw::rect(display_surface, bg_color, item_rect);
            }
            display_surface->Blit(text_surf, text_rect.pos);
            display_surface->Blit(icon_surf, icon_rect.pos);
        }
    }
}
