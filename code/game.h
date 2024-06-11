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
    void CreateSprite(const tmx_tile *tile, int posX, int posY);
    void CreateTileLayer(const tmx_map *map, const tmx_layer *layer);
    void Setup(const tmx_map *map, const std::string &player_start_position);

private:

    void UnloadResources();
    std::map<std::string, tmx_map *> tmx_maps;
    std::map<std::string, std::vector<Texture2D>> overworld_frames;
    AllSprites all_sprites{};
    Player *player{};
};
