#include "game.h"
#include "settings.h"
#include "sprite.h"

#include <memory>


Game::Game()
{
    rg::Init();
    display_surface = rg::display::SetMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    rg::display::SetCaption("RPG Monsters");

    all_sprites = AllSprites();
    ImportAssets();
    Setup("world", "house");
    // Setup("hospital", "world");
}

Game::~Game()
{
    UnloadResources();
}

void Game::run()
{
    while (!rl::WindowShouldClose())
    {
        const float dt = rg::time::Clock::tick();
        all_sprites.Update(dt);
        display_surface->Fill(rl::BLACK);
        all_sprites.Draw(player);
        rg::display::Update();
    }
}

void Game::ImportAssets()
{
    tmx_maps["world"] = rl::LoadTMX("resources/data/maps/world.tmx");
    tmx_maps["hospital"] = rl::LoadTMX("resources/data/maps/hospital.tmx");

    const auto waterList = rg::image::LoadFolderList("resources/graphics/tilesets/water");
    waterFrames = rg::Frames::Merge(waterList, 1, waterList.size());
}

void Game::Setup(const std::string &map_name, const std::string &player_start_position)
{
    const rl::tmx_map *map = tmx_maps[map_name];

    const rl::tmx_layer *terrain_layer = tmx_find_layer_by_name(map, "Terrain");
    const rl::tmx_layer *entities_layer = tmx_find_layer_by_name(map, "Entities");
    const rl::tmx_layer *objects_layer = tmx_find_layer_by_name(map, "Objects");
    const rl::tmx_layer *terrain_top_layer = tmx_find_layer_by_name(map, "Terrain Top");
    const rl::tmx_layer *water_layer = tmx_find_layer_by_name(map, "Water");

#if 0
    auto terrain_tiles = rg::tmx::GetTMXTiles(map, terrain_layer);
    for (auto &[position, texture, atlas_rect]: terrain_tiles)
    {
        auto surface = std::make_shared<rg::Surface>(texture, atlas_rect);
        std::make_shared<Sprite>(position, surface)->add(&all_sprites);
    }

    auto terrain_top_tiles = rg::tmx::GetTMXTiles(map, terrain_top_layer);
    for (auto &[position, texture, atlas_rect]: terrain_top_tiles)
    {
        auto surface = std::make_shared<rg::Surface>(texture, atlas_rect);
        std::make_shared<Sprite>(position, surface)->add(&all_sprites);
    }
#else
    const auto terrain_surf = rg::tmx::GetTMXLayerSurface(map, terrain_layer);
    std::make_shared<Sprite>(rg::math::Vector2{}, terrain_surf)->add(&all_sprites);

    const auto terrain_top_surf = rg::tmx::GetTMXLayerSurface(map, terrain_top_layer);
    std::make_shared<Sprite>(rg::math::Vector2{}, terrain_top_surf)->add(&all_sprites);
#endif

    // objects
    auto object = objects_layer->content.objgr->head;
    while (object)
    {
        const int gid = object->content.gid;
        if (map->tiles[gid])
        {
            rg::Rect atlas_rect;
            auto *tileTexture = rg::tmx::GetTMXTileTexture(map->tiles[gid], &atlas_rect);
            auto objSurf = std::make_shared<rg::Surface>(tileTexture, atlas_rect);
            auto position =
                    rg::math::Vector2{(float) object->x, (float) (object->y - object->height)};
            std::make_shared<Sprite>(position, objSurf)->add(&all_sprites);
        }
        object = object->next;
    }


    // we need to find the player first, so it can be passed to all characters in the next look
    auto entity = entities_layer->content.objgr->head;
    while (entity)
    {
        if (std::strcmp(entity->name, "Player") == 0)
        {
            const char *entity_pos = rl::tmx_get_property(entity->properties, "pos")->value.string;
            auto position = rg::math::Vector2{float(entity->x), float(entity->y)};
            if (std::strcmp(entity_pos, player_start_position.c_str()) == 0)
            {
                player = std::make_shared<Player>(position);
                player->add(&all_sprites);
            }
        }
        entity = entity->next;
    }

    // water
    auto water = water_layer->content.objgr->head;
    while (water)
    {
        for (int y = 0; y < water->height; y += TILE_SIZE)
        {
            for (int x = 0; x < water->width; x += TILE_SIZE)
            {
                auto position = rg::math::Vector2{float(x + water->x), float(y + water->y)};
                std::make_shared<AnimatedSprite>(position, waterFrames)->add(&all_sprites);
            }
        }
        water = water->next;
    }
}

void Game::UnloadResources()
{
    for (auto &[key, tmx_map]: tmx_maps)
    {
        UnloadTMX(tmx_map);
    }
}
