#include "monsterindex.h"

#include <raylib.h>


MonsterIndex::MonsterIndex(
        std::map<int, Monster> *monsters,
        const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts,
        std::map<std::string, std::shared_ptr<rg::Surface>> *monster_icons,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames)
    : monsters(monsters), fonts(fonts), monster_icons(monster_icons), monster_frames(monster_frames)
{
    tint_surf = std::make_shared<rg::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT);
    tint_surf->SetAlpha(200);
    main_rect.center(rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
}

void MonsterIndex::Update(const float dt)
{
    // input
    Input();
    // tint main game
    display_surface->Blit(tint_surf, {0, 0});
    // display the list
    DisplayList();
    // display the main section
    DisplayMain(dt);
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

    // lines
    for (int i = 1; i < std::min(visible_items, (int) monsters->size()); ++i)
    {
        auto y = main_rect.top() + item_height * i;
        auto left = main_rect.left();
        auto right = main_rect.left() + list_width;
        rg::draw::line(display_surface, COLORS["light-gray"], {left, y}, {right, y});
    }

    // shadow
    auto shadow_surf = std::make_shared<rg::Surface>(4, main_rect.height);
    shadow_surf->SetAlpha(100);
    display_surface->Blit(shadow_surf, {main_rect.left() + list_width - 4, main_rect.top()});
}

void MonsterIndex::DisplayMain(const double dt)
{
    // data
    const auto monster = (*monsters)[index];
    const auto element = NAMES_ELEMENT_TYPES[monster.element];

    // main bg
    const auto rect = rg::Rect{
            main_rect.left() + list_width, main_rect.top(), main_rect.width - list_width,
            main_rect.height};
    rg::draw::rect(display_surface, COLORS["dark"], rect, 0, 12, false, true, false, true);

    // monster display
    const auto top_rect =
            rg::Rect{rect.topleft(), rg::math::Vector2{rect.width, rect.height * 0.4f}};
    rg::draw::rect(display_surface, COLORS[element], top_rect, 0, 12, false, true, false, false);

    // monster animation
    frame_index += ANIMATION_SPEED * dt;
    auto monster_frame = (*monster_frames)[monster.name]["idle"];
    monster_frame->SetAtlas(frame_index);
    auto monster_rect = monster_frame->GetRect().center(top_rect.center());
    display_surface->Blit(monster_frame, monster_rect.pos);

    // name
    auto name_surf = fonts["bold"]->render(monster.name.c_str(), COLORS["white"]);
    auto name_rect = name_surf->GetRect().topleft(top_rect.topleft() + rg::math::Vector2{10, 10});
    display_surface->Blit(name_surf, name_rect.pos);

    // level
    auto level_surf =
            fonts["regular"]->render(rl::TextFormat("Lvl: %d", monster.level), COLORS["white"]);
    auto level_rect =
            level_surf->GetRect().bottomleft(top_rect.bottomleft() + rg::math::Vector2{10, -10});
    display_surface->Blit(level_surf, level_rect.pos);

    // element
    auto element_surf = fonts["regular"]->render(element.c_str(), COLORS["white"]);
    auto element_rect = element_surf->GetRect().bottomright(
            top_rect.bottomright() + rg::math::Vector2{-10, -10});
    display_surface->Blit(element_surf, element_rect.pos);
}
