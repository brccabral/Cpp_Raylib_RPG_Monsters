#include "game.h"
#include "settings.h"


Game::Game()
{
    rg::Init();
    display_surface = rg::display::SetMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    rg::display::SetCaption("RPG Monsters");
}

Game::~Game()
{
    rg::Quit();
}

void Game::run()
{
    while (!rl::WindowShouldClose())
    {
        display_surface->Fill(rl::RED);
        rg::display::Update();
    }
}
