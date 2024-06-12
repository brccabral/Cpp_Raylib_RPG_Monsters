#include "game.h"
#include <raylib.h>
#include "settings.h"
#include "sprite.h"

#define RAYLIB_TMX_IMPLEMENTATION
#include <cstring>

#include "entities.h"
#include "raylib-tmx.h"
#include "support.h"


Game::Game(const int width, const int height)
{
    // SetTraceLogLevel(LOG_ERROR);
    InitWindow(width, height, "RPG Monsters");
    SetTargetFPS(60);

    ImporAssets();

    Setup(tmx_maps["world"], "house");
    // Setup(tmx_maps["hospital"], "world");
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

    overworld_frames["water"] = ImportFolder("resources/graphics/tilesets/water");
    overworld_named_frames["coast"] = coast_importer(24, 12, "resources/graphics/tilesets/coast.png");
    overworld_named_frames["characters"] = all_character_import("resources/graphics/characters");
}

void Game::CreateSprite(const tmx_tile *tile, const int posX, const int posY)
{
    const tmx_image *im = tile->image;
    Texture2D *map_texture{};
    if (im && im->resource_image)
    {
        map_texture = (Texture2D *) im->resource_image;
    }
    else if (tile->tileset->image->resource_image)
    {
        map_texture = (Texture2D *) tile->tileset->image->resource_image;
    }
    if (map_texture)
    {
        const Vector2 position = {float(posX), float(posY)};
        Rectangle srcRect;
        srcRect.x = tile->ul_x;
        srcRect.y = tile->ul_y;
        srcRect.width = tile->width;
        srcRect.height = tile->height;

        new Sprite(position, map_texture, srcRect, &all_sprites);
    }
}

void Game::CreateTileLayer(const tmx_map *map, const tmx_layer *layer)
{
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            const unsigned int baseGid = layer->content.gids[(y * map->width) + x];
            const unsigned int gid = (baseGid) &TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid])
            {
                const tmx_tileset *ts = map->tiles[gid]->tileset;
                CreateSprite(map->tiles[gid], x * ts->tile_width, y * ts->tile_height);
            }
        }
    }
}
void Game::Setup(const tmx_map *map, const std::string &player_start_position)
{
    const tmx_layer *terrain_layer = tmx_find_layer_by_name(map, "Terrain");
    const tmx_layer *entities_layer = tmx_find_layer_by_name(map, "Entities");
    const tmx_layer *objects_layer = tmx_find_layer_by_name(map, "Objects");
    const tmx_layer *terrain_top_layer = tmx_find_layer_by_name(map, "Terrain Top");
    const tmx_layer *water_layer = tmx_find_layer_by_name(map, "Water");
    const tmx_layer *coast_layer = tmx_find_layer_by_name(map, "Coast");

    CreateTileLayer(map, terrain_layer);
    CreateTileLayer(map, terrain_top_layer);

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

    auto entity = entities_layer->content.objgr->head;
    while (entity)
    {
        if (strcmp(entity->name, "Player") == 0 &&
            strcmp(tmx_get_property(entity->properties, "pos")->value.string, player_start_position.c_str()) == 0)
        {
            player = new Player(
                    {float(entity->x), float(entity->y)}, overworld_named_frames["characters"]["player"], &all_sprites,
                    {float(0), float(0), float(128), float(128)});
        }

        entity = entity->next;
    }

    auto water = water_layer->content.objgr->head;
    while (water)
    {
        for (int y = 0; y < water->height; y += TILE_SIZE)
        {
            for (int x = 0; x < water->width; x += TILE_SIZE)
            {
                new AnimatedSprite({float(x + water->x), float(y + water->y)}, overworld_frames["water"], &all_sprites);
            }
        }
        water = water->next;
    }

    auto coast = coast_layer->content.objgr->head;
    while (coast)
    {
        std::string terrain = tmx_get_property(coast->properties, "terrain")->value.string;
        std::string side = tmx_get_property(coast->properties, "side")->value.string;
        new AnimatedSprite(
                {float(coast->x), float(coast->y)}, overworld_named_frames["coast"][terrain][side], &all_sprites);
        coast = coast->next;
    }
}

void Game::UnloadResources()
{
    UnloadTMX(tmx_maps["world"]);
}
