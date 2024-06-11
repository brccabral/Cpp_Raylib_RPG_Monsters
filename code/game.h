#pragma once
#include <map>
#include <string>
#include <tmx.h>
#include "groups.h"


class Game
{
public:

    Game(int width, int height);
    ~Game();
    void run();
    void ImporAssets();
    void Setup(tmx_map *map, const std::string &player_start_position);

private:

    void UnloadResources();
    std::map<std::string, tmx_map *> tmx_maps;
    AllSprites all_sprites{};
};
