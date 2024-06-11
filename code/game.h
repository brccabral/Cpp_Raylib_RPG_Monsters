#pragma once
#include "entities.h"


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
    void CreateSprite(const tmx_tile *tile, int x, int y);
    void Setup(tmx_map *map, const std::string &player_start_position);

private:

    void UnloadResources();
    std::map<std::string, tmx_map *> tmx_maps;
    AllSprites all_sprites{};
    Player *player;
};
