#include "monsterindex.h"


MonsterIndex::MonsterIndex(
        std::map<int, Monster> *monsters,
        const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts)
    // : monsters(monsters), fonts(fonts)
    : fonts(fonts)
{
    tint_surf = std::make_shared<rg::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT);
    tint_surf->SetAlpha(200);
}

void MonsterIndex::Update(float dt)
{
    // input
    display_surface->Blit(tint_surf, {0, 0});
    // tint main game
    // display the list
    // display the main section
}
