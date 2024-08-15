#include "monsterindex.h"


MonsterIndex::MonsterIndex(
        std::map<int, Monster> *monsters,
        const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts)
    : monsters(monsters), fonts(fonts)
{
    tint_surf = std::make_shared<rg::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT);
    tint_surf->SetAlpha(200);
    main_rect.center(rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
}

void MonsterIndex::Update(float dt)
{
    // input
    // tint main game
    display_surface->Blit(tint_surf, {0, 0});
    rg::draw::rect(display_surface, rl::BLACK, main_rect);
    // display the list
    DisplayList();
    // display the main section
}

void MonsterIndex::DisplayList()
{
    for (const auto &[index, monster]: *monsters)
    {
        const float top = main_rect.top() + index * item_height;
        const rg::Rect item_rect = {main_rect.x, top, list_width, item_height};

        auto text_surf = fonts["regular"]->render(monster.name.c_str(), COLORS["white"]);
        auto text_rect =
                text_surf->GetRect().midleft(item_rect.midleft() + rg::math::Vector2{90, 0});

        rg::draw::rect(display_surface, rl::RED, item_rect);
        display_surface->Blit(text_surf, text_rect.pos);
    }
}
