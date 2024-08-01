#pragma once
#include <rygame.h>


class Game
{
public:

    Game();
    ~Game();
    void run();

private:

    void ImportAssets();
    void Setup(const rl::tmx_map *tmx_map, std::string player_start_pos);
    void UnloadResources();

    rg::Surface *display_surface = nullptr;
    std::map<std::string, rl::tmx_map *> tmx_maps;
};
