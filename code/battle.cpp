#include "battle.h"

#include <iostream>

Battle::Battle(
        const std::vector<Monster> &player_monsters, const std::vector<Monster> &opponent_monsters,
        const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                &monsters_frames,
        const Texture2D &bg_surf, const std::map<std::string, Font> &fonts)
    : bg_surf(bg_surf), monsters_frames(monsters_frames), fonts(fonts),
      monster_data({{"player", player_monsters}, {"opponent", opponent_monsters}})
{
    Setup();
}

void Battle::Update(const double dt)
{
    BeginTextureMode(display_surface);
    DrawTexture(bg_surf, 0, 0, WHITE);
    EndTextureMode();
}

void Battle::Setup()
{
    for (auto &[entity, monsters]: monster_data)
    {
        for (int index = 0; index <= 2; ++index)
        {
            std::cout << monsters[index] << "\n";
        }
    }
}
