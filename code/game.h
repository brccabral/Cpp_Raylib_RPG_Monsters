#pragma once
#include <rygame.h>
#include "sprite.h"
#include "entities.h"
#include "groups.h"


class Game
{
public:

    Game();
    ~Game();
    void run();

    Player *player{};

private:

    void ImportAssets();
    void Setup(const rl::tmx_map *tmx_map, const std::string &player_start_pos);
    void UnloadResources();

    rg::Surface *display_surface = nullptr;
    std::map<std::string, rl::tmx_map *> tmx_maps;

    // Groups
    AllSprites all_sprites;
};
