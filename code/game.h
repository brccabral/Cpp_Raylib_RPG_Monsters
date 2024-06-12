#pragma once
#include <map>
#include <string>
#include <tmx.h>
#include "entities.h"
#include "groups.h"
#include "support.h"


class Game
{
public:

    Game(int width, int height);
    ~Game();
    void run();
    void ImporAssets();
    void CreateSprite(const tmx_tile *tile, int posX, int posY, int z = WORLD_LAYERS["main"]);
    void CreateTileLayer(const tmx_map *map, const tmx_layer *layer, int z = WORLD_LAYERS["main"]);
    void Setup(const tmx_map *map, const std::string &player_start_position);

private:

    void UnloadResources();
    std::map<std::string, tmx_map *> tmx_maps;
    std::map<std::string, std::vector<Texture2D>> overworld_frames;
    std::map<std::string, std::map<std::string, Texture2D>> named_textures;
    std::map<std::string, std::map<std::string, tilerect_name>> overworld_rect_frames;
    AllSprites all_sprites{};
    Player *player{};
};
