#include "monsterindex.hpp"


MonsterIndex::MonsterIndex(
        std::map<int, Monster> *monsters,
        std::unordered_map<std::string, rg::font::Font> *fonts,
        std::unordered_map<std::string, rg::Surface> *monster_icons,
        std::unordered_map<std::string, rg::Surface> *ui_icons,
        std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *
        monster_frames)
    : monsters(monsters), fonts(fonts), monster_icons(monster_icons),
      monster_frames(monster_frames), ui_icons(ui_icons)
{
    tint_surf = rg::Surface(WINDOW_WIDTH, WINDOW_HEIGHT);
    tint_surf.SetAlpha(200);
    main_rect.center(rg::math::Vector2{WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});

    for (auto &monster_data: GameData::GetInstance().MONSTER_DATA | std::views::values)
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
    display_surface->Blit(&tint_surf, rg::math::Vector2{0, 0});
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
            Monster tmp = std::move((*monsters)[selected_index]);
            (*monsters)[selected_index] = std::move((*monsters)[index]);
            (*monsters)[index] = std::move(tmp);
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
    const auto bg_rect = rg::Rect{main_rect.topleft(),
                                  rg::math::Vector2{list_width, main_rect.height}};
    rg::draw::rect(
            display_surface, Settings::GetInstance().COLORS["gray"], bg_rect, 0, 12, true, false,
            true, false);

    const float v_offset = (index < visible_items) ? 0 : -(index - visible_items + 1) * item_height;
    monster_name_list_surfaces.clear();
    monster_name_list_surfaces.reserve(monsters->size());
    for (const auto &[list_index, monster]: *monsters)
    {
        // colors
        const auto bg_color = (index != list_index)
                                  ? Settings::GetInstance().COLORS["gray"]
                                  : Settings::GetInstance().COLORS["light"];
        const auto text_color = (selected_index != list_index)
                                    ? Settings::GetInstance().COLORS["white"]
                                    : Settings::GetInstance().COLORS["gold"];

        const float top = main_rect.top() + list_index * item_height + v_offset;
        const rg::Rect item_rect = {main_rect.x, top, list_width, item_height};

        auto name_list_surf = (*fonts)["regular"].render(monster.name.c_str(), text_color);
        auto text_rect =
                name_list_surf.GetRect().midleft(
                        item_rect.midleft() + rg::math::Vector2{90.0f, 0.0f});
        monster_name_list_surfaces.push_back(std::move(name_list_surf));

        auto *icon_surf = &(*monster_icons)[monster.name];
        auto icon_rect =
                icon_surf->GetRect().center(item_rect.midleft() + rg::math::Vector2{45.0f, 0.0f});

        if (item_rect.colliderect(main_rect))
        {
            // check corners
            if (item_rect.collidepoint(main_rect.topleft()))
            {
                rg::draw::rect(
                        display_surface, bg_color, item_rect, 0, 12, true, false, false, false);
            }
            else if (item_rect.collidepoint(
                    main_rect.bottomleft() + rg::math::Vector2{1.0f, -1.0f}))
            {
                rg::draw::rect(
                        display_surface, bg_color, item_rect, 0, 12, false, false, true, false);
            }
            else
            {
                rg::draw::rect(display_surface, bg_color, item_rect);
            }
            display_surface->Blit(&monster_name_list_surfaces.back(), text_rect);
            display_surface->Blit(icon_surf, icon_rect);
        }
    }

    // lines
    for (int i = 1; i < std::min(visible_items, (int) monsters->size()); ++i)
    {
        auto y = main_rect.top() + item_height * i;
        auto left = main_rect.left();
        auto right = main_rect.left() + list_width;
        rg::draw::line(
                display_surface, Settings::GetInstance().COLORS["light-gray"], {left, y},
                {right, y});
    }

    // shadow
    shadow_surf = rg::Surface(4, int(main_rect.height));
    shadow_surf.SetAlpha(100);
    display_surface->Blit(
            &shadow_surf, rg::math::Vector2{main_rect.left() + list_width - 4, main_rect.top()});
}

void MonsterIndex::DisplayMain(const float dt)
{
    // data
    auto *monster = &(*monsters)[index];
    const auto element = GameData::GetInstance().NAMES_ELEMENT_TYPES[monster->element];

    // main bg
    const auto rect = rg::Rect{
            main_rect.left() + list_width, main_rect.top(), main_rect.width - list_width,
            main_rect.height};
    rg::draw::rect(
            display_surface, Settings::GetInstance().COLORS["dark"], rect, 0, 12, false, true,
            false, true);

    // monster display
    const auto top_rect =
            rg::Rect{rect.topleft(), rg::math::Vector2{rect.width, rect.height * 0.4f}};
    rg::draw::rect(
            display_surface, Settings::GetInstance().COLORS[element], top_rect, 0, 12, false, true,
            false, false);

    // monster animation
    frame_index += ANIMATION_SPEED * dt;
    auto *monster_frame = &(*monster_frames)[monster->name][ANIMATIONSTATE_IDLE];
    monster_frame->SetAtlas(frame_index);
    auto monster_rect = monster_frame->GetRect().center(top_rect.center());
    display_surface->Blit(monster_frame, monster_rect);

    // name
    name_surf = (*fonts)["bold"].render(
            monster->name.c_str(), Settings::GetInstance().COLORS["white"]);
    auto name_rect = name_surf.GetRect().topleft(
            top_rect.topleft() + rg::math::Vector2{10.0f, 10.0f});
    display_surface->Blit(&name_surf, name_rect);

    // level
    level_surf =
            (*fonts)["regular"].render(
                    rl::TextFormat("Lvl: %d", monster->level),
                    Settings::GetInstance().COLORS["white"]);
    auto level_rect =
            level_surf.GetRect().bottomleft(
                    top_rect.bottomleft() + rg::math::Vector2{10.0f, -16.0f});
    display_surface->Blit(&level_surf, level_rect);

    // xp bar
    rg::draw::bar(
            display_surface, {level_rect.bottomleft(), {100, 4}}, monster->xp, monster->level_up,
            Settings::GetInstance().COLORS["white"], Settings::GetInstance().COLORS["dark"]);

    // element
    element_surf = (*fonts)["regular"].render(
            element.c_str(), Settings::GetInstance().COLORS["white"]);
    auto element_rect = element_surf.GetRect().bottomright(
            top_rect.bottomright() + rg::math::Vector2{-10.0f, -10.0f});
    display_surface->Blit(&element_surf, element_rect);

    // health and energy
    float bar_width = rect.width * 0.45f;
    float bar_height = 30;
    float bar_top = top_rect.bottom() + rect.width * 0.03f;
    float bar_leftside = rect.left() + rect.width / 4;
    float bar_rightside = rect.left() + rect.width * 3.0f / 4.0f;

    auto health_rect = rg::Rect{0.0f, 0.0f, bar_width, bar_height};
    health_rect.midtop(rg::math::Vector2{bar_leftside, bar_top});
    rg::draw::bar(
            display_surface, health_rect, monster->health, monster->GetStat("max_health"),
            Settings::GetInstance().COLORS["red"], Settings::GetInstance().COLORS["black"], 2);
    hp_text = (*fonts)["regular"].render(
            rl::TextFormat("HP: %.0f/%.0f", monster->health, monster->GetStat("max_health")),
            Settings::GetInstance().COLORS["white"]);
    auto hp_rect = hp_text.GetRect().midleft(
            health_rect.midleft() + rg::math::Vector2{10.0f, 0.0f});
    display_surface->Blit(&hp_text, hp_rect);

    auto energy_rect = rg::Rect{0.0f, 0.0f, bar_width, bar_height};
    energy_rect.midtop(rg::math::Vector2{bar_rightside, bar_top});
    rg::draw::bar(
            display_surface, energy_rect, monster->energy, monster->GetStat("max_energy"),
            Settings::GetInstance().COLORS["blue"], Settings::GetInstance().COLORS["black"], 2);
    ep_text = (*fonts)["regular"].render(
            rl::TextFormat("EP: %.0f/%.0f", monster->energy, monster->GetStat("max_energy")),
            Settings::GetInstance().COLORS["white"]);
    auto ep_rect = ep_text.GetRect().midleft(
            energy_rect.midleft() + rg::math::Vector2{10.0f, 0.0f});
    display_surface->Blit(&ep_text, ep_rect);

    // info
    auto info_height = rect.bottom() - health_rect.bottom();

    // stats
    auto stats_rect =
            rg::Rect{health_rect.left(), health_rect.bottom(), health_rect.width, info_height}
            .inflate(0, -60)
            .move({0, 15});
    stats_text_surf = (*fonts)["regular"].render(
            "Stats", Settings::GetInstance().COLORS["white"]);
    auto stats_text_rect = stats_text_surf.GetRect().bottomleft(stats_rect.topleft());
    display_surface->Blit(&stats_text_surf, stats_text_rect);

    auto monster_stats = monster->GetStats();
    float stat_height = stats_rect.height / monster_stats.size();
    monster_stats_sufaces.clear();
    monster_stats_sufaces.reserve(monster_stats.size());
    int i = 0;
    for (auto &[stat, value]: monster_stats)
    {
        auto single_stat_rect = rg::Rect{
                stats_rect.left(), stats_rect.top() + i * stat_height, stats_rect.width,
                stat_height};

        // icon
        auto *icon_surf = &(*ui_icons)[stat];
        auto icon_rect =
                icon_surf->GetRect().midleft(
                        single_stat_rect.midleft() + rg::math::Vector2{5.0f, 0.0f});
        display_surface->Blit(icon_surf, icon_rect);

        // text
        auto monster_stat_surf = (*fonts)["regular"].render(
                stat.c_str(), Settings::GetInstance().COLORS["white"]);
        auto text_rect =
                monster_stat_surf.GetRect().topleft(
                        icon_rect.topright() + rg::math::Vector2{10.0f, -5.0f});
        monster_stats_sufaces.push_back(std::move(monster_stat_surf));
        display_surface->Blit(&monster_stats_sufaces.back(), text_rect);

        // bar
        auto bar_rect = rg::Rect{
                text_rect.left(), text_rect.bottom() + 2,
                single_stat_rect.width - (text_rect.left() - single_stat_rect.left()), 4.0f};
        rg::draw::bar(
                display_surface, bar_rect, value, max_stats[stat] * monster->level,
                Settings::GetInstance().COLORS["white"],
                Settings::GetInstance().COLORS["black"]);

        ++i;
    }

    // abilities
    auto ability_rect = stats_rect.copy().left(energy_rect.left());
    ability_text_surf = (*fonts)["regular"].render(
            "Ability", Settings::GetInstance().COLORS["white"]);
    auto ability_text_rect = ability_text_surf.GetRect().bottomleft(ability_rect.topleft());
    display_surface->Blit(&ability_text_surf, ability_text_rect);

    auto abilities = monster->GetAbilities();
    abilities_surfaces.clear();
    abilities_surfaces.reserve(abilities.size());
    for (unsigned int a_index = 0; a_index < abilities.size(); ++a_index)
    {
        auto a_element = GameData::GetInstance().ATTACK_DATA[abilities[a_index]].element;

        auto ability_name_surf = (*fonts)["regular"].render(
                GameData::GetInstance().ATTACK_DATA[abilities[a_index]].name.c_str(),
                Settings::GetInstance().COLORS["black"]);
        float x = ability_rect.left() + a_index % 2 * ability_rect.width / 2;
        float y =
                20.0f + ability_rect.top() + int(a_index / 2) * (
                    ability_name_surf.GetRect().height + 20);
        auto a_rect = ability_name_surf.GetRect().topleft(rg::math::Vector2{x, y});
        abilities_surfaces.push_back(std::move(ability_name_surf));
        rg::draw::rect(
                display_surface,
                Settings::GetInstance().COLORS[GameData::GetInstance().NAMES_ELEMENT_TYPES[
                    a_element]], a_rect.inflate(10, 10), 0,
                4);
        display_surface->Blit(&abilities_surfaces.back(), a_rect);
    }
}
