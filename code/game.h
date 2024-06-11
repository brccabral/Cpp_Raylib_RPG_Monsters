#pragma once
#include "sprite.h"


#include <map>
#include <string>
#include <tmx.h>
#include <vector>


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
    SpriteGroup all_sprites{};
};
