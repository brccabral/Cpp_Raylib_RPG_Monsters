#include "game.h"
#include "settings.h"


Game::Game()
{
    rg::Init();
    display_surface = rg::display::SetMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    rg::display::SetCaption("RPG Monsters");

    ImportAssets();
    Setup(tmx_maps["world"], "house");
}

Game::~Game()
{
    UnloadResources();
}

void Game::run()
{
    while (!rl::WindowShouldClose())
    {
        all_sprites.Draw(display_surface);
        rg::display::Update();
    }
}

void Game::ImportAssets()
{
    tmx_maps["world"] = rl::LoadTMX("resources/data/maps/world.tmx");
}

void Game::Setup(const rl::tmx_map *tmx_map, const std::string &player_start_position)
{
    const rl::tmx_layer *terrain_layer = tmx_find_layer_by_name(tmx_map, "Terrain");
    const rl::tmx_layer *entities_layer = tmx_find_layer_by_name(tmx_map, "Entities");

    auto *terrain_surface = rg::tmx::GetTMXLayerSurface(tmx_map, terrain_layer);
    new Sprite({}, terrain_surface, {&all_sprites});

    // we need to find the player first, so it can be passed to all characters in the next look
    auto entity = entities_layer->content.objgr->head;
    while (entity)
    {
        if (std::strcmp(entity->name, "Player") == 0)
        {
            const char *entity_pos = rl::tmx_get_property(entity->properties, "pos")->value.string;
            if (std::strcmp(entity_pos, player_start_position.c_str()) == 0)
            {
                player = new Player({float(entity->x), float(entity->y)}, {&all_sprites});
                player->rect.topleft({10, 10});
            }
        }
        entity = entity->next;
    }
}

void Game::UnloadResources()
{
    for (auto &[key, tmx_map]: tmx_maps)
    {
        UnloadTMX(tmx_map);
    }
}
