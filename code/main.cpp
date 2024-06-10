#include <raylib.h>
#include "settings.h"

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RPG Monsters");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        const Color bg = COLORS[std::string("gold")];
        ClearBackground(bg);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
