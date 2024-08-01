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
    rg::Quit();
}

void Game::run()
{
    while (!rl::WindowShouldClose())
    {
        rg::display::Update();
    }
}

void Game::ImportAssets()
{
    tmx_maps["world"] = rl::LoadTMX("resources/data/maps/world.tmx");
}

void Game::Setup(const rl::tmx_map *tmx_map, std::string player_start_pos)
{
    const auto *terrain_layer = tmx_find_layer_by_name(tmx_map, "Terrain");
    const auto terrain_surface = rg::tmx::GetTMXLayerSurface(tmx_map, terrain_layer);
    display_surface->Blit(terrain_surface);
}

void Game::UnloadResources()
{
    for (auto &[key, tmx_map]: tmx_maps)
    {
        UnloadTMX(tmx_map);
    }
}
