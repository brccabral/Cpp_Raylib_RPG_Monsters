#include "monsterindex.h"

#include "raylib_utils.h"
#include "settings.h"


MonsterIndex::MonsterIndex(
        const std::vector<Monster> &monsters, const std::map<std::string, Font> &fonts,
        std::map<std::string, std::map<std::string, Texture2D>> monster_frames)
    : fonts(fonts), monsters(monsters)
{
    RectToCenter(main_rect, {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});

    tint_surface = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    BeginTextureMode(tint_surface);
    ClearBackground(BLACK);
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 200.0f / 255.0f));
    DrawRectangleRec(main_rect, BLACK);
    EndTextureMode();

    icon_frames = monster_frames["icons"];
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
        const auto [x, y] = GetRectMidLeft(item_rect);

        const Texture2D icon_texture = icon_frames[monsters[i].name];

        // BeginTextureMode was called in Update()
        DrawRectangleRec(item_rect, Fade(COLORS["red"], (i + 1.0f) / monsters.size()));
        DrawTexture(icon_texture, x + 45 - icon_texture.width / 2.0f, y - icon_texture.height / 2.0f, WHITE);
        DrawTextEx(
                fonts["regular"], monsters[i].name.c_str(), {x + 90, y - fonts["regular"].baseSize / 2.0f},
                fonts["regular"].baseSize, 2, COLORS["white"]);
    }
}
