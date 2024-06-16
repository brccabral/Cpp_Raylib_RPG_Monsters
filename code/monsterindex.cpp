#include "monsterindex.h"
#include "settings.h"


MonsterIndex::MonsterIndex(std::vector<Monster> monsters, const std::map<std::string, Font> &fonts) : fonts(fonts)
{
    tint_surface = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    BeginTextureMode(tint_surface);
    ClearBackground(BLACK);
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 200.0f / 255.0f));
    EndTextureMode();
}

void MonsterIndex::Update(double dt)
{
    // input

    // tint main game
    BeginTextureMode(display_surface);
    DrawTexture(tint_surface.texture, 0, 0, WHITE);
    EndTextureMode();

    // display list

    // display the main section
}
