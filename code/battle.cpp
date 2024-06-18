#include "battle.h"

Battle::Battle(
        const std::vector<Monster> &player_monsters, const std::vector<Monster> &opponent_monsters,
        const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                &monsters_frames,
        const Texture2D &bg_surf, const std::map<std::string, Font> &fonts)
    : bg_surf(bg_surf), monsters_frames(monsters_frames), fonts(fonts),
      monster_data({{"player", player_monsters}, {"opponent", opponent_monsters}})
{}

void Battle::Update(const double dt)
{
    BeginTextureMode(display_surface);
    DrawTexture(bg_surf, 0, 0, WHITE);
    EndTextureMode();
}
