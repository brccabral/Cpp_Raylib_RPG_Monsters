#include "monsterindex.hpp"


MonsterIndex::MonsterIndex(
        std::map<int, Monster> *monsters,
        const std::map<std::string, std::shared_ptr<rg::font::Font>> &fonts,
        std::map<std::string, std::shared_ptr<rg::Surface>> *monster_icons,
        std::map<std::string, std::shared_ptr<rg::Surface>> *ui_icons,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames)
    : monsters(monsters), fonts(fonts), monster_icons(monster_icons),
      monster_frames(monster_frames), ui_icons(ui_icons)
{
    tint_surf = std::make_shared<rg::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT);
    tint_surf->SetAlpha(200);
    main_rect.center(rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});

    for (auto &[monster_name, monster_data]: MONSTER_DATA)
    {
        for (auto &[stat, value]: monster_data.stats)
        {
            max_stats[stat] = (value > max_stats[stat]) ? value : max_stats[stat];
        }
    }
    max_stats["health"] = max_stats["max_health"];
    max_stats["energy"] = max_stats["max_energy"];
}

void MonsterIndex::Update(const float dt)
{
    // input
    Input();
    // tint main game
    display_surface->Blit(tint_surf, {0, 0});
    // display the list
    DisplayList();
    // display the main section
    DisplayMain(dt);
}

void MonsterIndex::Input()
{
    if (IsKeyPressed(rl::KEY_UP))
    {
        index -= 1;
    }
    if (IsKeyPressed(rl::KEY_DOWN))
    {
        index += 1;
    }
    const int size = (int) monsters->size();
    index = (index % size + size) % size;

    if (IsKeyPressed(rl::KEY_SPACE))
    {
        // one SPACE to select, another to change order
        if (selected_index != -1)
        {
            // second SPACE is to change the order
            const auto selected_monster = (*monsters)[selected_index];
            const auto current_monster = (*monsters)[index];
            (*monsters)[index] = selected_monster;
            (*monsters)[selected_index] = current_monster;
            selected_index = -1;
        }
        else
        {
            // first SPACE is to select the monster from the list
            selected_index = index;
        }
    }
}

void MonsterIndex::DisplayList()
{
    float v_offset = (index < visible_items) ? 0 : -(index - visible_items + 1) * item_height;
    for (const auto &[list_index, monster]: *monsters)
    {
        // colors
        auto bg_color = (index != list_index) ? COLORS["gray"] : COLORS["light"];
        auto text_color = (selected_index != list_index) ? COLORS["white"] : COLORS["gold"];

        const float top = main_rect.top() + list_index * item_height + v_offset;
        const rg::Rect item_rect = {main_rect.x, top, list_width, item_height};

        auto text_surf = fonts["regular"]->render(monster.name.c_str(), text_color);
        auto text_rect =
                text_surf->GetRect().midleft(item_rect.midleft() + rg::math::Vector2{90, 0});

        auto icon_surf = (*monster_icons)[monster.name];
        auto icon_rect =
                icon_surf->GetRect().center(item_rect.midleft() + rg::math::Vector2{45, 0});

        if (item_rect.colliderect(main_rect))
        {
            // check corners
            if (item_rect.collidepoint(main_rect.topleft()))
            {
                rg::draw::rect(
                        display_surface, bg_color, item_rect, 0, 12, true, false, false, false);
            }
            else if (item_rect.collidepoint(main_rect.bottomleft() + rg::math::Vector2{1, -1}))
            {
                rg::draw::rect(
                        display_surface, bg_color, item_rect, 0, 12, false, false, true, false);
            }
            else
            {
                rg::draw::rect(display_surface, bg_color, item_rect);
            }
            display_surface->Blit(text_surf, text_rect.pos);
            display_surface->Blit(icon_surf, icon_rect.pos);
        }
    }

    // lines
    for (int i = 1; i < std::min(visible_items, (int) monsters->size()); ++i)
    {
        auto y = main_rect.top() + item_height * i;
        auto left = main_rect.left();
        auto right = main_rect.left() + list_width;
        rg::draw::line(display_surface, COLORS["light-gray"], {left, y}, {right, y});
    }

    // shadow
    auto shadow_surf = std::make_shared<rg::Surface>(4, main_rect.height);
    shadow_surf->SetAlpha(100);
    display_surface->Blit(shadow_surf, {main_rect.left() + list_width - 4, main_rect.top()});
}

void MonsterIndex::DisplayMain(const double dt)
{
    // data
    auto monster = (*monsters)[index];
    const auto element = NAMES_ELEMENT_TYPES[monster.element];

    // main bg
    const auto rect = rg::Rect{
            main_rect.left() + list_width, main_rect.top(), main_rect.width - list_width,
            main_rect.height};
    rg::draw::rect(display_surface, COLORS["dark"], rect, 0, 12, false, true, false, true);

    // monster display
    const auto top_rect =
            rg::Rect{rect.topleft(), rg::math::Vector2{rect.width, rect.height * 0.4f}};
    rg::draw::rect(display_surface, COLORS[element], top_rect, 0, 12, false, true, false, false);

    // monster animation
    frame_index += ANIMATION_SPEED * dt;
    auto monster_frame = (*monster_frames)[monster.name]["idle"];
    monster_frame->SetAtlas(frame_index);
    auto monster_rect = monster_frame->GetRect().center(top_rect.center());
    display_surface->Blit(monster_frame, monster_rect.pos);

    // name
    auto name_surf = fonts["bold"]->render(monster.name.c_str(), COLORS["white"]);
    auto name_rect = name_surf->GetRect().topleft(top_rect.topleft() + rg::math::Vector2{10, 10});
    display_surface->Blit(name_surf, name_rect.pos);

    // level
    auto level_surf =
            fonts["regular"]->render(rl::TextFormat("Lvl: %d", monster.level), COLORS["white"]);
    auto level_rect =
            level_surf->GetRect().bottomleft(top_rect.bottomleft() + rg::math::Vector2{10, -16});
    display_surface->Blit(level_surf, level_rect.pos);

    // xp bar
    rg::draw::bar(
            display_surface, {level_rect.bottomleft(), {100, 4}}, monster.xp, monster.level_up,
            COLORS["white"], COLORS["dark"]);

    // element
    auto element_surf = fonts["regular"]->render(element.c_str(), COLORS["white"]);
    auto element_rect = element_surf->GetRect().bottomright(
            top_rect.bottomright() + rg::math::Vector2{-10, -10});
    display_surface->Blit(element_surf, element_rect.pos);

    // health and energy
    float bar_width = rect.width * 0.45f;
    float bar_height = 30;
    float bar_top = top_rect.bottom() + rect.width * 0.03f;
    float bar_leftside = rect.left() + rect.width / 4;
    float bar_rightside = rect.left() + rect.width * 3.0f / 4.0f;

    auto health_rect = rg::Rect{0, 0, bar_width, bar_height};
    health_rect.midtop(rg::math::Vector2{bar_leftside, bar_top});
    rg::draw::bar(
            display_surface, health_rect, monster.health, monster.GetStat("max_health"),
            COLORS["red"], COLORS["black"], 2);
    auto hp_text = fonts["regular"]->render(
            rl::TextFormat("HP: %.0f/%.0f", monster.health, monster.GetStat("max_health")),
            COLORS["white"]);
    auto hp_rect = hp_text->GetRect().midleft(health_rect.midleft() + rg::math::Vector2{10, 0});
    display_surface->Blit(hp_text, hp_rect.pos);

    auto energy_rect = rg::Rect{0, 0, bar_width, bar_height};
    energy_rect.midtop(rg::math::Vector2{bar_rightside, bar_top});
    rg::draw::bar(
            display_surface, energy_rect, monster.energy, monster.GetStat("max_energy"),
            COLORS["blue"], COLORS["black"], 2);
    auto ep_text = fonts["regular"]->render(
            rl::TextFormat("EP: %.0f/%.0f", monster.energy, monster.GetStat("max_energy")),
            COLORS["white"]);
    auto ep_rect = ep_text->GetRect().midleft(energy_rect.midleft() + rg::math::Vector2{10, 0});
    display_surface->Blit(ep_text, ep_rect.pos);

    // info
    auto info_height = rect.bottom() - health_rect.bottom();

    // stats
    auto stats_rect =
            rg::Rect{health_rect.left(), health_rect.bottom(), health_rect.width, info_height}
                    .inflate(0, -60)
                    .move({0, 15});
    auto stats_text_surf = fonts["regular"]->render("Stats", COLORS["white"]);
    auto stats_text_rect = stats_text_surf->GetRect().bottomleft(stats_rect.topleft());
    display_surface->Blit(stats_text_surf, stats_text_rect.pos);

    auto monster_stats = monster.GetStats();
    float stat_height = stats_rect.height / monster_stats.size();
    int i = 0;
    for (auto &[stat, value]: monster_stats)
    {
        auto single_stat_rect = rg::Rect{
                stats_rect.left(), stats_rect.top() + i * stat_height, stats_rect.width,
                stat_height};

        // icon
        auto icon_surf = (*ui_icons)[stat];
        auto icon_rect =
                icon_surf->GetRect().midleft(single_stat_rect.midleft() + rg::math::Vector2{5, 0});
        display_surface->Blit(icon_surf, icon_rect.pos);

        // text
        auto text_surf = fonts["regular"]->render(stat.c_str(), COLORS["white"]);
        auto text_rect =
                text_surf->GetRect().topleft(icon_rect.topright() + rg::math::Vector2{10, -5});
        display_surface->Blit(text_surf, text_rect.pos);

        // bar
        auto bar_rect = rg::Rect{
                text_rect.left(), text_rect.bottom() + 2, single_stat_rect.width * 0.9f, 4};
        rg::draw::bar(
                display_surface, bar_rect, value, max_stats[stat] * monster.level, COLORS["white"],
                COLORS["black"]);

        ++i;
    }

    // abilities
    auto ability_rect = stats_rect.copy().left(energy_rect.left());
    auto ability_text_surf = fonts["regular"]->render("Ability", COLORS["white"]);
    auto ability_text_rect = ability_text_surf->GetRect().bottomleft(ability_rect.topleft());
    display_surface->Blit(ability_text_surf, ability_text_rect.pos);

    auto abilities = monster.GetAbilities();
    for (int a_index = 0; a_index < abilities.size(); ++a_index)
    {
        auto a_element = ATTACK_DATA[abilities[a_index]].element;

        auto text_surf = fonts["regular"]->render(
                ATTACK_DATA[abilities[a_index]].name.c_str(), COLORS["black"]);
        float x = ability_rect.left() + a_index % 2 * ability_rect.width / 2;
        float y =
                20.0f + ability_rect.top() + int(a_index / 2) * (text_surf->GetRect().height + 20);
        auto a_rect = text_surf->GetRect().topleft(rg::math::Vector2{x, y});
        rg::draw::rect(
                display_surface, COLORS[NAMES_ELEMENT_TYPES[a_element]], a_rect.inflate(10, 10), 0,
                4);
        display_surface->Blit(text_surf, a_rect.pos);
    }
}
