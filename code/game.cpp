#include "game.h"
#include <raylib.h>
#include "settings.h"

Game::Game(const int width, const int height)
{
    InitWindow(width, height, "RPG Monsters");
    SetTargetFPS(60);
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
