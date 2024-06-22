#include "monsterindex.h"
#include "game_data.h"
#include "raylib_utils.h"


MonsterIndex::MonsterIndex(
        const std::vector<Monster> &monsters, const std::map<std::string, Font> &fonts,
        const std::map<std::string, Texture2D> &monster_icons,
        const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                &monsters_frms,
        const std::map<std::string, Texture2D> &ui_frms)
    : fonts(fonts), monsters(monsters), icon_frames(monster_icons), ui_frames(ui_frms),
      monsters_frames(monsters_frms)
{
    RectToCenter(main_rect, {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});

    tint_surface = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    while (!IsRenderTextureReady(tint_surface))
    {}
    BeginTextureModeC(tint_surface, BLANK);
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 200.0f / 255.0f));
    EndTextureModeSafe();
}

MonsterIndex::~MonsterIndex()
{
    UnloadRenderTexture(tint_surface);
}

void MonsterIndex::Update(const double dt)
{
    // input
    Input();

    // we draw on top of main game
    BeginTextureModeSafe(display_surface); // we don't want to clear what is there

    // tint main game
    DrawTexture(tint_surface.texture, 0, 0, WHITE);
    // display list
    DisplayList();
    // display the main section
    DisplayMain(dt);

    EndTextureModeSafe();
}

std::map<std::string, float> MonsterIndex::GetMaxStats()
{
    std::map<std::string, float> max_stats;
    for (auto &[monster_name, monster_data]: MONSTER_DATA)
    {
        for (auto &[stat, value]: monster_data.stats)
        {
            max_stats[stat] = (value > max_stats[stat]) ? value : max_stats[stat];
        }
    }
    max_stats["health"] = max_stats["max_health"];
    max_stats["energy"] = max_stats["max_energy"];
    return max_stats;
}

void MonsterIndex::DisplayList()
{
    // BeginTextureMode was called in Update()
    const RectangleU bg_rect = {main_rect.pos, {list_width, main_rect.height}};
    DrawRectangleRoundedCorners(bg_rect, 0.1, 10, COLORS["gray"], true, false, false, true);

    // vertical offset
    const int v_offset = (index < visible_items) ? 0 : -(index - visible_items + 1) * item_height;
    for (int i = 0; i < monsters.size(); ++i)
    {
        const Color bg_color = (i != index) ? COLORS["gray"] : COLORS["light"];
        const Color text_color = (selected_index != i) ? COLORS["white"] : COLORS["gold"];
        const float top = main_rect.y + i * item_height + v_offset;
        const RectangleU item_rect = {main_rect.x, top, list_width, item_height};
        const auto [x, y] = GetRectMidLeft(item_rect);

        const Texture2D icon_texture = icon_frames[monsters[i].name];

        if (CheckCollisionRecs(item_rect.rectangle, main_rect.rectangle))
        {
            if (CheckCollisionPointRec(GetRectTopLeft(main_rect), item_rect.rectangle))
            {
                DrawRectangleRoundedCorners(
                        item_rect, 0.3, 10, bg_color, true, false, false, false);
            }
            else if (CheckCollisionPointRec(
                             Vector2Add(GetRectBottomLeft(main_rect), {1, -1}),
                             item_rect.rectangle))
            {
                DrawRectangleRoundedCorners(
                        item_rect, 0.3, 10, bg_color, false, false, false, true);
            }
            else
            {
                DrawRectangleRec(item_rect.rectangle, bg_color);
            }
            DrawTexture(
                    icon_texture, x + 45 - icon_texture.width / 2.0f,
                    y - icon_texture.height / 2.0f, WHITE);
            DrawTextEx(
                    fonts["regular"], monsters[i].name.c_str(),
                    {x + 90, y - fonts["regular"].baseSize / 2.0f}, fonts["regular"].baseSize, 2,
                    text_color);
        }
    }

    // lines between monsters
    for (int i = 1; i < std::min(visible_items, (int) monsters.size()); ++i)
    {
        const float y = main_rect.y + item_height * i;
        DrawLine(main_rect.x, y, main_rect.x + list_width, y, COLORS["light-gray"]);
    }

    // shadow
    DrawRectangle(
            main_rect.x + list_width - 4, main_rect.y, 4, main_rect.height,
            Fade(BLACK, 100.0f / 255.0f));
}

void MonsterIndex::DisplayMain(const double dt)
{
    // data
    Monster monster = monsters[index];

    // BeginTextureMode was called in Update()
    // bg
    const RectangleU rect = {
            main_rect.x + list_width, main_rect.y, main_rect.width - list_width, main_rect.height};
    DrawRectangleRoundedCorners(rect, 0.1, 10, COLORS["dark"], false, true, true, false);

    // monster
    const RectangleU top_rect = {rect.pos, rect.width, rect.height * 0.4f};
    DrawRectangleRoundedCorners(
            top_rect, 0.1, 10, COLORS[monster.element], false, true, false, false);

    // monster animation
    frame_index += ANIMATION_SPEED * dt;
    const auto frames = monsters_frames[monster.name]["idle"];
    const auto [surf_texture, surf_ref] = frames[int(frame_index) % frames.size()];
    RectangleU monster_rect = surf_ref;
    RectToCenter(monster_rect, GetRectCenter(top_rect));
    DrawTextureRec(*surf_texture, surf_ref.rectangle, monster_rect.pos, WHITE);

    // name
    DrawTextEx(
            fonts["bold"], monster.name.c_str(), {top_rect.x + 10, top_rect.y + 10}, 14, 1,
            COLORS["white"]);

    // level
    const Vector2 pos =
            Vector2Add(GetRectBottomLeft(top_rect), {10.0f, -16.0f - fonts["regular"].baseSize});
    DrawTextEx(
            fonts["regular"], TextFormat("Lvl: %i", monster.level), pos, fonts["regular"].baseSize,
            1, COLORS["white"]);
    DrawBar({pos.x, pos.y + fonts["regular"].baseSize, 100, 4}, monster.xp, monster.level_up,
            COLORS["white"], COLORS["dark"]);

    // element
    const Vector2 size_element = MeasureTextF(fonts["regular"], monster.element.c_str(), 1);
    const Vector2 pos_element = Vector2Subtract(
            Vector2Add(GetRectBottomRight(top_rect), {-10.0f, -10.0f}), size_element);
    DrawTextEx(
            fonts["regular"], monster.element.c_str(), pos_element, fonts["regular"].baseSize, 1,
            COLORS["white"]);

    // health and energy
    const RectangleU bar_rect{
            rect.x + rect.width * 0.03f, top_rect.y + top_rect.height + rect.width * 0.03f,
            rect.width * 0.45f, 30.0f};

    const RectangleU health_rectangle = bar_rect;
    DrawBar(health_rectangle, monster.health, monster.GetStat("max_health"), COLORS["red"],
            COLORS["black"], 0.4f, 20);
    DrawTextEx(
            fonts["regular"],
            TextFormat("HP: %.f/%.f", monster.health, monster.GetStat("max_health")),
            Vector2Add(GetRectMidLeft(health_rectangle), {10, -fonts["regular"].baseSize / 2.0f}),
            fonts["regular"].baseSize, 1, COLORS["white"]);

    const RectangleU energy_rectangle{bar_rect.x + rect.width / 2.0f, bar_rect.y, bar_rect.size};
    DrawBar(energy_rectangle, monster.energy, monster.GetStat("max_energy"), COLORS["blue"],
            COLORS["black"], 0.4f, 20);
    DrawTextEx(
            fonts["regular"],
            TextFormat("EP: %.f/%.f", monster.energy, monster.GetStat("max_energy")),
            Vector2Add(GetRectMidLeft(energy_rectangle), {10, -fonts["regular"].baseSize / 2.0f}),
            fonts["regular"].baseSize, 1, COLORS["white"]);

    // info
    const float info_height = rect.y + rect.height - (health_rectangle.y + health_rectangle.height);

    // stats
    RectangleU stats_rectangle = {
            health_rectangle.x, health_rectangle.y + health_rectangle.height,
            health_rectangle.width, info_height};
    RectInflate(stats_rectangle, 0, -60);
    MoveRect(stats_rectangle, {0, 15});
    const Vector2 stats_pos = GetRectTopLeft(stats_rectangle);
    DrawTextEx(
            fonts["regular"], "Stats", {stats_pos.x, stats_pos.y - fonts["regular"].baseSize},
            fonts["regular"].baseSize, 1, COLORS["white"]);

    auto monster_stats = monster.GetStats();
    float stat_height = stats_rectangle.height / monster_stats.size();
    int i = 0;
    for (auto &[stat, value]: monster_stats)
    {
        RectangleU single_stat_rectangle = {
                stats_rectangle.x, stats_rectangle.y + i * stat_height, stats_rectangle.width,
                stat_height};

        // icon
        Texture2D icon_surf = ui_frames[stat];
        RectangleU icon_rect = {0, 0, (float) icon_surf.width, (float) icon_surf.height};
        RectToMidLeft(icon_rect, Vector2Add(GetRectMidLeft(single_stat_rectangle), {5, 0}));
        DrawTextureV(icon_surf, GetRectTopLeft(icon_rect), WHITE);

        // text
        Vector2 stat_text_pos = Vector2Add(GetRectTopLeft(icon_rect), {icon_rect.width + 14, -5});
        DrawTextEx(
                fonts["regular"], stat.c_str(), stat_text_pos, fonts["regular"].baseSize, 1,
                COLORS["white"]);

        // bar
        RectangleU stat_bar_rect = single_stat_rectangle;
        stat_bar_rect.height = 4;
        stat_bar_rect.pos = Vector2Add(stat_text_pos, {0, fonts["regular"].baseSize + 2.0f});
        stat_bar_rect.size =
                Vector2Add(stat_bar_rect.size, {-(stat_text_pos.x - single_stat_rectangle.x), 4});
        DrawBar(stat_bar_rect, value, max_stats[stat] * monster.level, COLORS["white"],
                COLORS["black"]);

        ++i;
    }

    // abilities
    RectangleU abilities_rect = stats_rectangle;
    abilities_rect.x = energy_rectangle.x;
    const Vector2 abilities_pos = GetRectTopLeft(abilities_rect);
    DrawTextEx(
            fonts["regular"], "Ability",
            {abilities_pos.x, abilities_pos.y - fonts["regular"].baseSize},
            fonts["regular"].baseSize, 1, COLORS["white"]);

    auto abilities = monster.GetAbilities();
    for (int a_index = 0; a_index < abilities.size(); ++a_index)
    {
        auto element = ATTACK_DATA[abilities[a_index]].element;
        float x = abilities_rect.x + a_index % 2 * abilities_rect.width / 2;
        float y = 20.f + abilities_rect.y + int(a_index / 2) * (fonts["regular"].baseSize + 20);
        Vector2 ability_pos = {x, y};
        Vector2 ability_text_size = MeasureTextF(fonts["regular"], abilities[a_index].c_str(), 1);
        RectangleU ability_rect = {ability_pos, ability_text_size};
        RectInflate(ability_rect, 10, 10);

        DrawRectangleRoundedCorners(
                ability_rect, 0.3f, 10, COLORS[element], true, true, true, true);
        DrawTextEx(
                fonts["regular"], abilities[a_index].c_str(), ability_pos,
                fonts["regular"].baseSize, 1, COLORS["black"]);
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
