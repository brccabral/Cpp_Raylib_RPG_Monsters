#include "game.h"
#include <iostream>
#include <raylib.h>
#include "settings.h"
#include "sprite.h"

#define RAYLIB_TMX_IMPLEMENTATION
#include <cstring>

#include "entities.h"
#include "raylib-tmx.h"


Game::Game(const int width, const int height)
{
    InitWindow(width, height, "RPG Monsters");
    // SetTargetFPS(60);
    ImporAssets();
    // Setup(tmx_maps["world"], "house");
    Setup(tmx_maps["hospital"], "world");
}

Game::~Game()
{
    for (const auto sprite: all_sprites.sprites)
    {
        delete sprite;
    }
    UnloadResources();
    CloseWindow();
}

void Game::run()
{
    while (!WindowShouldClose())
    {
        all_sprites.Update(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);

        all_sprites.Draw(player->GetCenter());

        EndDrawing();
    }
}

void Game::ImporAssets()
{
    tmx_map *world = LoadTMX("resources/data/maps/world.tmx");
    tmx_maps["world"] = world;
    tmx_map *hospital = LoadTMX("resources/data/maps/hospital.tmx");
    tmx_maps["hospital"] = hospital;
}

void Game::CreateSprite(const tmx_tile *tile, const int posX, const int posY)
{
    const tmx_image *im = tile->image;
    Texture2D *image;
    if (im && im->resource_image)
    {
        image = (Texture2D *) im->resource_image;
    }
    else if (tile->tileset->image->resource_image)
    {
        image = (Texture2D *) tile->tileset->image->resource_image;
    }
    if (image)
    {
        const Vector2 position = {float(posX), float(posY)};
        Rectangle srcRect;
        srcRect.x = tile->ul_x;
        srcRect.y = tile->ul_y;
        srcRect.width = tile->width;
        srcRect.height = tile->height;
        new Sprite(position, image, &all_sprites, srcRect);
    }
}
void Game::Setup(tmx_map *map, const std::string &player_start_position)
{
    const tmx_layer *terrain_layer = tmx_find_layer_by_name(map, "Terrain");
    const tmx_layer *entities_layer = tmx_find_layer_by_name(map, "Entities");
    const tmx_layer *objects_layer = tmx_find_layer_by_name(map, "Objects");
    const tmx_layer *terrain_top_layer = tmx_find_layer_by_name(map, "Terrain Top");

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            const unsigned int baseGid = terrain_layer->content.gids[(y * map->width) + x];
            const unsigned int gid = (baseGid) &TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid])
            {
                const tmx_tileset *ts = map->tiles[gid]->tileset;
                CreateSprite(map->tiles[gid], x * ts->tile_width, y * ts->tile_height);
            }
        }
    }

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            const unsigned int baseGid = terrain_top_layer->content.gids[(y * map->width) + x];
            const unsigned int gid = (baseGid) &TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid])
            {
                const tmx_tileset *ts = map->tiles[gid]->tileset;
                CreateSprite(map->tiles[gid], x * ts->tile_width, y * ts->tile_height);
            }
        }
    }

    auto entity = entities_layer->content.objgr->head;
    while (entity)
    {
        if (strcmp(entity->name, "Player") == 0 &&
            strcmp(tmx_get_property(entity->properties, "pos")->value.string, player_start_position.c_str()) == 0)
        {
            const Rectangle dest = {float(entity->x), float(entity->y), float(entity->width), float(entity->height)};
            Texture2D playertx_texture = LoadTexture("resources/graphics/characters/young_guy.png");
            player = new Player({float(entity->x), float(entity->y)}, &playertx_texture, &all_sprites, dest);
        }

        entity = entity->next;
    }

    auto object = objects_layer->content.objgr->head;
    while (object)
    {
        const int gid = object->content.gid;
        if (map->tiles[gid])
        {
            CreateSprite(map->tiles[gid], object->x, object->y - object->height);
        }
        object = object->next;
    }
}

void Game::UnloadResources()
{
    UnloadTMX(tmx_maps["world"]);
}
