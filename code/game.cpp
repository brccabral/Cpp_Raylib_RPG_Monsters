#include "game.h"
#include "settings.h"
#include "sprite.h"


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

void Game::Setup(const rl::tmx_map *tmx_map, const std::string &player_start_pos)
{
    const auto *terrain_layer = tmx_find_layer_by_name(tmx_map, "Terrain");
    auto *terrain_surface = rg::tmx::GetTMXLayerSurface(tmx_map, terrain_layer);
    new Sprite({}, terrain_surface, {&all_sprites});
}

void Game::UnloadResources()
{
    for (auto &[key, tmx_map]: tmx_maps)
    {
        UnloadTMX(tmx_map);
    }
}
