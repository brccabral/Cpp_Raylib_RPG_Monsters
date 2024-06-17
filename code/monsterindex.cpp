#include "monsterindex.h"

#include "raylib_utils.h"
#include "settings.h"


MonsterIndex::MonsterIndex(
        const std::vector<Monster> &monsters, const std::map<std::string, Font> &fonts,
        const std::map<std::string, Texture2D> &monster_icons,
        const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>> &monsters_frms,
        const std::map<std::string, Texture2D> &ui_frms)
    : fonts(fonts), monsters(monsters), icon_frames(monster_icons), monsters_frames(monsters_frms), ui_frames(ui_frms)
{
    RectToCenter(main_rect, {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});

    tint_surface = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    BeginTextureMode(tint_surface);
    ClearBackground(BLACK);
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 200.0f / 255.0f));
    EndTextureMode();
}

void MonsterIndex::Update(const double dt)
{
    // input
    Input();

    BeginTextureMode(display_surface);

    // tint main game
    DrawTexture(tint_surface.texture, 0, 0, WHITE);
    // display list
    DisplayList();
    // display the main section
    DisplayMain(dt);

    EndTextureMode();
}

void MonsterIndex::DisplayList()
{
    // BeginTextureMode was called in Update()
    // vertical offset
    const int v_offset = (index < visible_items) ? 0 : -(index - visible_items + 1) * item_height;
    for (int i = 0; i < monsters.size(); ++i)
    {
        const Color bg_color = (i != index) ? COLORS["gray"] : COLORS["light"];
        const Color text_color = (selected_index != i) ? COLORS["white"] : COLORS["gold"];
        const float top = main_rect.y + i * item_height + v_offset;
        const Rectangle item_rect = {main_rect.x, top, list_width, item_height};
        const auto [x, y] = GetRectMidLeft(item_rect);

        const Texture2D icon_texture = icon_frames[monsters[i].name];

        if (CheckCollisionRecs(item_rect, main_rect))
        {
            if (CheckCollisionPointRec(GetRectTopLeft(main_rect), item_rect))
            {
                DrawRectangleRoundedCorners(item_rect, 0.3, 10, bg_color, true, false, false, false);
            }
            else if (CheckCollisionPointRec(Vector2Add(GetRectBottomLeft(main_rect), {1, -1}), item_rect))
            {
                DrawRectangleRoundedCorners(item_rect, 0.3, 10, bg_color, false, false, false, true);
            }
            else
            {
                DrawRectangleRec(item_rect, bg_color);
            }
            DrawTexture(icon_texture, x + 45 - icon_texture.width / 2.0f, y - icon_texture.height / 2.0f, WHITE);
            DrawTextEx(
                    fonts["regular"], monsters[i].name.c_str(), {x + 90, y - fonts["regular"].baseSize / 2.0f},
                    fonts["regular"].baseSize, 2, text_color);
        }
    }

    // lines between monsters
    for (int i = 1; i < std::min(visible_items, (int) monsters.size()); ++i)
    {
        const float y = main_rect.y + item_height * i;
        DrawLine(main_rect.x, y, main_rect.x + list_width, y, COLORS["light-gray"]);
    }

    // shadow
    DrawRectangle(main_rect.x + list_width - 4, main_rect.y, 4, main_rect.height, Fade(BLACK, 100.0f / 255.0f));
}

void MonsterIndex::DisplayMain(const double dt)
{
    // data
    Monster monster = monsters[index];

    // BeginTextureMode was called in Update()
    // bg
    const Rectangle rect = {main_rect.x + list_width, main_rect.y, main_rect.width - list_width, main_rect.height};
    DrawRectangleRoundedCorners(rect, 0.3, 10, COLORS["dark"], false, true, true, false);

    // monster
    const Rectangle top_rect = {rect.x, rect.y, rect.width, rect.height * 0.4f};
    DrawRectangleRoundedCorners(top_rect, 0.3, 10, COLORS[monster.element], false, true, false, false);

    // monster animation
    frame_index += ANIMATION_SPEED * dt;
    const auto frames = monsters_frames[monster.name]["idle"];
    const auto [surf_texture, surf_ref] = frames[int(frame_index) % frames.size()];
    Rectangle monster_rect = surf_ref;
    RectToCenter(monster_rect, GetRectCenter(top_rect));
    DrawTextureRec(*surf_texture, surf_ref, {monster_rect.x, monster_rect.y}, WHITE);

    // name
    DrawTextEx(fonts["bold"], monster.name.c_str(), {top_rect.x + 10, top_rect.y + 10}, 14, 1, COLORS["white"]);

    // level
    const Vector2 pos = Vector2Add(GetRectBottomLeft(top_rect), {10.0f, -16.0f - fonts["regular"].baseSize});
    DrawTextEx(
            fonts["regular"], TextFormat("Lvl: %i", monster.level), pos, fonts["regular"].baseSize, 1, COLORS["white"]);
    DrawBar({pos.x, pos.y + fonts["regular"].baseSize, 100, 4}, monster.xp, monster.level_up, COLORS["white"],
            COLORS["dark"]);

    // element
    const Vector2 size_element = MeasureTextEx(fonts["regular"], monster.element.c_str(), fonts["regular"].baseSize, 1);
    const Vector2 pos_element =
            Vector2Subtract(Vector2Add(GetRectBottomRight(top_rect), {-10.0f, -10.0f}), size_element);
    DrawTextEx(fonts["regular"], monster.element.c_str(), pos_element, fonts["regular"].baseSize, 1, COLORS["white"]);

    // health and energy
    const Rectangle bar_rect{
            rect.x + rect.width * 0.03f, top_rect.y + top_rect.height + rect.width * 0.03f, rect.width * 0.45f, 30.0f};

    const Rectangle health_rectangle{bar_rect.x, bar_rect.y, bar_rect.width, bar_rect.height};
    DrawBar(health_rectangle, monster.health, float(monster.GetStat("max_health")), COLORS["red"], COLORS["black"],
            100);
    DrawTextEx(
            fonts["regular"], TextFormat("HP: %i/%i", monster.health, monster.GetStat("max_health")),
            Vector2Add(GetRectMidLeft(health_rectangle), {10, -fonts["regular"].baseSize / 2.0f}),
            fonts["regular"].baseSize, 1, COLORS["white"]);

    const Rectangle energy_rectangle{bar_rect.x + rect.width / 2.0f, bar_rect.y, bar_rect.width, bar_rect.height};
    DrawBar(energy_rectangle, monster.energy, float(monster.GetStat("max_energy")), COLORS["blue"], COLORS["black"],
            100);
    DrawTextEx(
            fonts["regular"], TextFormat("EP: %i/%i", monster.energy, monster.GetStat("max_energy")),
            Vector2Add(GetRectMidLeft(energy_rectangle), {10, -fonts["regular"].baseSize / 2.0f}),
            fonts["regular"].baseSize, 1, COLORS["white"]);

    // info
    const float info_height = rect.y + rect.height - (health_rectangle.y + health_rectangle.height);

    // stats
    Rectangle stats_rectangle = {
            health_rectangle.x, health_rectangle.y + health_rectangle.height, health_rectangle.width, info_height};
    RectInflate(stats_rectangle, 0, -60);
    const Vector2 stats_pos = GetRectTopLeft(stats_rectangle);
    DrawTextEx(
            fonts["regular"], "Stats", {stats_pos.x, stats_pos.y - fonts["regular"].baseSize},
            fonts["regular"].baseSize, 1, COLORS["white"]);
    DrawRectangleRec(stats_rectangle, Fade(RED, 0.5f));

    auto monster_stats = monster.GetStats();
    float stat_height = stats_rectangle.height / monster_stats.size();
    int i = 0;
    for (auto &[stat, value]: monster_stats)
    {
        Rectangle single_stat_rectangle = {
                stats_rectangle.x, stats_rectangle.y + i * stat_height, stats_rectangle.width, stat_height};

        // icon
        Texture2D icon_surf = ui_frames[stat];
        Rectangle icon_rect = {0, 0, (float) icon_surf.width, (float) icon_surf.height};
        RectToMidLeft(icon_rect, Vector2Add(GetRectMidLeft(single_stat_rectangle), {5, 0}));
        DrawTextureV(icon_surf, GetRectTopLeft(icon_rect), WHITE);

        // text
        DrawTextEx(
                fonts["regular"], stat.c_str(), Vector2Add(GetRectTopLeft(icon_rect), {icon_rect.width + 14, -5}),
                fonts["regular"].baseSize, 1, COLORS["white"]);

        ++i;
    }
}

void MonsterIndex::Input()
{
    if (IsKeyPressed(KEY_UP))
    {
        --index;
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        ++index;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        // one SPACE to select, another to change order
        if (selected_index != -1)
        {
            // second SPACE is to change the order
            const auto selected_monster = monsters[selected_index];
            const auto current_monster = monsters[index];
            monsters[index] = selected_monster;
            monsters[selected_index] = current_monster;
            selected_index = -1;
        }
        else
        {
            // first SPACE is to select the monster from the list
            selected_index = index;
        }
    }
    index %= monsters.size();
}
