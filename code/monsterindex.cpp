#include "monsterindex.h"

#include "raylib_utils.h"
#include "settings.h"


MonsterIndex::MonsterIndex(const std::vector<Monster> &monsters, const std::map<std::string, Font> &fonts)
    : fonts(fonts), monsters(monsters)
{
    RectToCenter(main_rect, {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});

    tint_surface = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    BeginTextureMode(tint_surface);
    ClearBackground(BLACK);
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 200.0f / 255.0f));
    DrawRectangleRec(main_rect, BLACK);
    EndTextureMode();
}

void MonsterIndex::Update(double dt)
{
    // input

    BeginTextureMode(display_surface);
    // tint main game
    DrawTexture(tint_surface.texture, 0, 0, WHITE);

    // display list
    DisplayList();
    EndTextureMode();

    // display the main section
}

void MonsterIndex::DisplayList()
{
    for (int i = 0; i < monsters.size(); ++i)
    {
        const float top = main_rect.y + i * item_height;
        const Rectangle item_rect = {main_rect.x, top, list_width, item_height};
        const Vector2 pos = GetRectMidLeft(item_rect);
        DrawTextEx(fonts["regular"], monsters[i].name.c_str(), pos, fonts["regular"].baseSize, 2, COLORS["white"]);
        // BeginTextureMode was called in Update()
        DrawRectangleRec(item_rect, Fade(COLORS["red"], (i + 1.0f) / monsters.size()));
    }
}
