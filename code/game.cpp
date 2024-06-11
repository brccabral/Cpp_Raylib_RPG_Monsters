#include "game.h"
#include <raylib.h>
#include "settings.h"

#define RAYLIB_TMX_IMPLEMENTATION
#include <iostream>


#include "raylib-tmx.h"

Game::Game(const int width, const int height)
{
    InitWindow(width, height, "RPG Monsters");
    SetTargetFPS(60);
    ImporAssets();
    Setup(tmx_maps["world"], "house");
}

Game::~Game()
{
    CloseWindow();
}

void Game::run()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        const Color bg = COLORS[std::string("gold")];
        ClearBackground(bg);

        EndDrawing();
    }
}

void Game::ImporAssets()
{
    tmx_map *world = LoadTMX("resources/data/maps/world.tmx");
    tmx_maps["world"] = world;
}

void Game::Setup(const tmx_map *map, const std::string &player_start_position)
{
    // const auto tilecount = map->tilecount;
    std::cout << "Map"
              << "\n";
    std::cout << "Width " << map->width << "\n";
    std::cout << "Height " << map->height << "\n";
    std::cout << "Count " << map->tilecount << "\n";
    std::cout << "Order " << map->renderorder << "\n";

    std::cout << "Layer"
              << "\n";
    const tmx_layer *layer = tmx_find_layer_by_name(map, "Terrain");

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            const unsigned int baseGid = layer->content.gids[(y * map->width) + x];
            const unsigned int gid = (baseGid) &TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid] && map->tiles[gid]->tileset && map->tiles[gid]->tileset->image)
            {
                std::cout << "x " << x;
                std::cout << " y " << y;
                std::cout << " sx " << map->tiles[gid]->width;
                std::cout << " sy " << map->tiles[gid]->height;
                std::cout << " posx " << x * map->tiles[gid]->width;
                std::cout << " posy " << y * map->tiles[gid]->height;
                std::cout << map->tiles[gid]->tileset->image->source << "\n";
            }
        }
    }
}

void Game::UnloadResources()
{
    UnloadTMX(tmx_maps["world"]);
}
