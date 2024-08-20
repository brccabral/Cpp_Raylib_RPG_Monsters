#include "battle.hpp"


Battle::Battle(
        std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames,
        const std::shared_ptr<rg::Surface> &bg_surf,
        std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts)
    : player_monsters(player_monsters), opponent_monsters(opponent_monsters),
      monster_frames(monster_frames), bg_surf(bg_surf), fonts(fonts)
{}

void Battle::Update(float dt)
{
    int a = player_monsters->size();
    int b = opponent_monsters->size();
    int c = monster_frames->size();
    int d = bg_surf->GetRect().width;
    int e = fonts->size();
    int p = a + b + c + d + e;
    display_surface->Blit(bg_surf, {float(p - p), 0});
}
