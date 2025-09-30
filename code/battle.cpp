#include "battle.hpp"


Battle::Battle(
        std::map<int, Monster> *player_monsters,
        std::map<int, Monster> *opponent_monsters,
        std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *
        monster_frames,
        std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *
        outline_frames,
        std::unordered_map<std::string, rg::Surface> *monster_icons,
        std::unordered_map<std::string, rg::Surface> *ui_icons,
        std::unordered_map<AttackAnimation, rg::Frames> *attack_frames, const rg::Surface *bg_surf,
        std::unordered_map<std::string, rg::font::Font> *fonts,
        const std::function<void(Character *c)> &endBattle,
        Character *character,
        std::unordered_map<std::string, rg::mixer::Sound> *sounds)
    : battle_over(false), player_monsters(player_monsters),
      opponent_monsters(opponent_monsters), monster_frames(monster_frames),
      outline_frames(outline_frames),
      monster_icons(monster_icons), ui_icons(ui_icons), bg_surf(bg_surf), fonts(fonts),
      attack_frames(attack_frames), endBattle(endBattle), character(character), sounds(sounds)
{
    indexes[SELECTMODE_GENERAL] = 0;
    indexes[SELECTMODE_MONSTER] = 0;
    indexes[SELECTMODE_ATTACKS] = 0;
    indexes[SELECTMODE_SWITCH] = 0;
    indexes[SELECTMODE_TARGET] = 0;

    timers["opponent_delay"] = rg::Timer(
            0.6f, false, false, [this]
            {
                OpponentAttack();
            });

    Setup();
}

Battle::Battle(Battle &&other) noexcept
    : Battle()
{
    *this = std::move(other);
}

Battle &Battle::operator=(Battle &&other) noexcept
{
    if (this != &other)
    {
        battle_over = other.battle_over;
        display_surface = other.display_surface;
        player_monsters = other.player_monsters;
        opponent_monsters = other.opponent_monsters;
        monster_frames = other.monster_frames;
        outline_frames = other.outline_frames;
        monster_icons = other.monster_icons;
        ui_icons = other.ui_icons;
        bg_surf = other.bg_surf;
        fonts = other.fonts;
        battle_sprites = std::move(other.battle_sprites);
        player_sprites = std::move(other.player_sprites);
        opponent_sprites = std::move(other.opponent_sprites);
        battle_monster = std::move(other.battle_monster);
        timed_sprites_ = std::move(other.timed_sprites_);
        attack_sprites_ = std::move(other.attack_sprites_);
        current_monster = other.current_monster;
        selection_mode = other.selection_mode;
        selection_side = other.selection_side;
        indexes = other.indexes;
        available_monsters = other.available_monsters;
        opponent_extra_monsters = other.opponent_extra_monsters;
        selected_attack = other.selected_attack;
        attack_frames = other.attack_frames;
        timers = std::move(other.timers);
        monsters_paused = other.monsters_paused;
        endBattle = other.endBattle;
        character = other.character;
        sounds = other.sounds;

        timers["opponent_delay"].func = [this]
        {
            OpponentAttack();
        };

        for (auto &bm: battle_monster)
        {
            bm.monster_sprite.apply_attack = [this](
                    const MonsterSprite *target_sprite, const Attack attack,
                    const float amount)
                    {
                        ApplyAttack(target_sprite, attack, amount);
                    };
            bm.monster_sprite.createMonster = [this](
                    Monster *monster_, const int index_, const int pos_index_,
                    const SelectionSide entity_)
                    {
                        CreateMonster(monster_, index_, pos_index_, entity_);
                    };
        }
    }
    return *this;
}

void Battle::Update(const float dt)
{
    CheckEndBattle();
    Input();
    UpdateTimers();
    // updates
    battle_sprites.Update(dt);
    CheckActive();

    // drawing
    display_surface->Blit(bg_surf, rg::math::Vector2{0, 0});
    battle_sprites.Draw(
            current_monster, selection_side, selection_mode, indexes[SELECTMODE_TARGET],
            &player_sprites, &opponent_sprites);
    DrawUi();
}

void Battle::Setup()
{
    battle_sprites.reserve(6 * 10);
    player_sprites.reserve(6 * 10);
    opponent_sprites.reserve(6 * 10);
    monsters_paused = false;
    battle_monster.resize(6 * 10);
    timed_sprites_.resize(6 * 10);
    attack_sprites_.resize(6 * 10);
    std::vector<int> added_opponents;
    for (auto &[index, monster]: *player_monsters)
    {
        if (index <= 2)
        {
            CreateMonster(&monster, index, index, PLAYER);
        }
    }
    for (auto &[index, monster]: *opponent_monsters)
    {
        if (index <= 2)
        {
            CreateMonster(&monster, index, index, OPPONENT);
            added_opponents.push_back(index);
        }
    }
    // save extra monsters
    for (auto &[index, monster]: *opponent_monsters)
    {
        if (std::ranges::find(added_opponents, index) == added_opponents.end())
        {
            opponent_extra_monsters[index] = &monster;
        }
    }
}

void Battle::CreateMonster(
        Monster *monster, const int index, const int pos_index, const SelectionSide entity)
{
    auto &frames = (*monster_frames)[monster->name];
    auto &outlines = (*outline_frames)[monster->name];
    rg::math::Vector2 pos;
    auto groups = std::vector<rg::sprite::Group *>{};

    if (entity == PLAYER)
    {
        pos = Settings::GetInstance().BATTLE_POSITIONS["left"][pos_index];
        groups = {&battle_sprites, &player_sprites};
        for (auto &[state, frame]: frames)
        {
            frames[state] = rg::transform::Flip(&frame, true, false);
        }
        for (auto &[state, frame]: outlines)
        {
            outlines[state] = rg::transform::Flip(&frame, true, false);
        }
    }
    else
    {
        pos = Settings::GetInstance().BATTLE_POSITIONS["right"][pos_index];
        groups = {&battle_sprites, &opponent_sprites};
    }
    for (auto &bm: battle_monster)
    {
        if (!bm.monster_sprite.monster)
        {
            bm.monster_sprite = MonsterSprite(
                    pos, &frames, monster, index, pos_index, entity,
                    [this](
                    const MonsterSprite *target_sprite, const Attack attack,
                    const float amount)
                    {
                        ApplyAttack(target_sprite, attack, amount);
                    },
                    [this](
                    Monster *monster_, const int index_, const int pos_index_,
                    const SelectionSide entity_)
                    {
                        CreateMonster(monster_, index_, pos_index_, entity_);
                    });
            bm.monster_sprite.add(groups);
            bm.monster_sprite.monster->paused = monsters_paused;

            bm.monster_outline_sprite = MonsterOutlineSprite(&bm.monster_sprite, &outlines);
            bm.monster_outline_sprite.add(
                    &battle_sprites);

            // ui
            rg::math::Vector2 name_pos;

            if (entity == PLAYER)
            {
                name_pos = bm.monster_sprite.rect.midleft() + rg::math::Vector2{16, -70};
            }
            else
            {
                name_pos = bm.monster_sprite.rect.midright() + rg::math::Vector2{-40, -70};
            }
            bm.monster_name_sprite =
                    MonsterNameSprite(name_pos, &bm.monster_sprite, &(*fonts)["regular"]);
            bm.monster_name_sprite.add(&battle_sprites);

            rg::math::Vector2 anchor;
            if (entity == PLAYER)
            {
                anchor = bm.monster_name_sprite.rect.bottomleft();
            }
            else
            {
                anchor = bm.monster_name_sprite.rect.bottomright();
            }

            bm.monster_level_sprite = MonsterLevelSprite(
                    entity, anchor, &bm.monster_sprite, &(*fonts)["small"]);
            bm.monster_level_sprite.add(&battle_sprites);

            bm.monster_stats_sprite = MonsterStatsSprite(
                    bm.monster_sprite.rect.midbottom() + rg::math::Vector2{0, 20},
                    &bm.monster_sprite,
                    rg::math::Vector2{150, 48}, &(*fonts)["small"]);
            bm.monster_stats_sprite.add(&battle_sprites);

            break;
        }
    }
}

void Battle::CheckActive()
{
    CheckActiveGroup(&player_sprites, PLAYER);
    CheckActiveGroup(&opponent_sprites, OPPONENT);
}

void Battle::CheckActiveGroup(rg::sprite::Group *group, const SelectionSide side)
{
    if (group->Sprites().empty())
    {
        return;
    }
    for (auto *sprite: group->Sprites())
    {
        const auto monster_sprite = dynamic_cast<MonsterSprite *>(sprite);
        if (monster_sprite->monster->initiative >= 100)
        {
            UpdateAllMonsters(true);
            monster_sprite->monster->initiative = 0;
            monster_sprite->SetHighlight(true);
            current_monster = monster_sprite;
            if (side == PLAYER)
            {
                selection_mode = SELECTMODE_GENERAL;
                selection_side = PLAYER;
            }
            else
            {
                timers["opponent_delay"].Activate();
            }
        }
    }
}

void Battle::UpdateAllMonsters(const bool do_pause)
{
    monsters_paused = do_pause;
    for (auto *sprite: player_sprites.Sprites())
    {
        const auto monster_sprite = dynamic_cast<MonsterSprite *>(sprite);
        monster_sprite->monster->paused = do_pause;
    }
    for (auto *sprite: opponent_sprites.Sprites())
    {
        const auto monster_sprite = dynamic_cast<MonsterSprite *>(sprite);
        monster_sprite->monster->paused = do_pause;
    }
}

void Battle::Input()
{
    if (selection_mode && current_monster)
    {
        int limiter = 0;
        switch (selection_mode)
        {
            case SELECTMODE_GENERAL:
            {
                limiter = Settings::GetInstance().BATTLE_CHOICES["full"].size();
                break;
            }
            case SELECTMODE_ATTACKS:
            {
                limiter = current_monster->monster->GetAbilities(false).size();
                // in case monster A has more attacks than monster B
                if (limiter)
                {
                    indexes[SELECTMODE_ATTACKS] = indexes[SELECTMODE_ATTACKS] % limiter;
                }
                break;
            }
            case SELECTMODE_SWITCH:
            {
                limiter = available_monsters.size();
                break;
            }
            case SELECTMODE_TARGET:
            {
                // some attacks like "defense"/"healing" are targeting the player
                limiter = selection_side == OPPONENT
                              ? opponent_sprites.Sprites().size()
                              : player_sprites.Sprites().size();
                break;
            }
            case SELECTMODE_NONE:
            case SELECTMODE_MONSTER:
            default:
                break;
        }
        if (IsKeyPressed(rl::KEY_DOWN))
        {
            indexes[selection_mode] =
                    limiter ? ((indexes[selection_mode] + 1) % limiter + limiter) % limiter : 0;
        }
        if (IsKeyPressed(rl::KEY_UP))
        {
            indexes[selection_mode] =
                    limiter ? ((indexes[selection_mode] - 1) % limiter + limiter) % limiter : 0;
        }
        if (IsKeyPressed(rl::KEY_SPACE))
        {
            if (selection_mode == SELECTMODE_SWITCH)
            {
                if (!available_monsters.empty())
                {
                    std::vector<std::pair<int, Monster *>> list_available;
                    list_available.reserve(available_monsters.size());
                    for (const auto &[index, monster]: available_monsters)
                    {
                        list_available.emplace_back(index, monster);
                    }

                    const int newIndex = list_available[indexes[SELECTMODE_SWITCH]].first;
                    const auto newMonster = list_available[indexes[SELECTMODE_SWITCH]].second;
                    current_monster->Kill();
                    CreateMonster(newMonster, newIndex, current_monster->pos_index, PLAYER);
                    selection_mode = SELECTMODE_NONE;
                    UpdateAllMonsters(false);
                }
            }
            else if (selection_mode == SELECTMODE_TARGET)
            {
                auto *sprite_group =
                        selection_side == PLAYER ? &player_sprites : &opponent_sprites;
                // when a monster gets defeated, the group may change, but the "pos_index" won't
                // create a list ordered by "pos_index"
                auto ordered_pos = sprite_group->Sprites(); // force a copy
                std::ranges::sort(
                        ordered_pos,
                        [](const auto &a, const auto &b)
                        {
                            return dynamic_cast<MonsterSprite *>(a)->pos_index <
                                   dynamic_cast<MonsterSprite *>(b)->pos_index;
                        });
                const auto monster_sprite = dynamic_cast<MonsterSprite *>(
                    ordered_pos[indexes[SELECTMODE_TARGET]]);
                if (selected_attack)
                {
                    current_monster->ActivateAttack(monster_sprite, selected_attack);
                    selected_attack = ATTACK_NONE;
                    current_monster = nullptr;
                    selection_mode = SELECTMODE_NONE;
                }
                else
                {
                    if (monster_sprite->monster->health <
                        monster_sprite->monster->GetStat("max_health") *
                        0.9f) // TODO 0.9f is for testing, lower it
                    {
                        // catching monster
                        // when deleting, set to PLAYER to not delete `Monster *`
                        monster_sprite->entity = PLAYER;
                        const auto it = std::ranges::find_if(
                                *opponent_monsters,
                                [monster_sprite](auto const &kv)
                                {
                                    return &kv.second == monster_sprite->monster;
                                });
                        (*player_monsters)[player_monsters->size()] = std::move(it->second);
                        opponent_monsters->erase(it);
                        monster_sprite->DelayedKill(
                                nullptr, 0, 0,
                                OPPONENT); // kills the MonsterSprite*, not the Monster*
                        UpdateAllMonsters(false);
                    }
                    else
                    {
                        for (auto &timed: timed_sprites_)
                        {
                            if (!timed.death_timer.active)
                            {
                                // can't catch monster, show a red X
                                timed = TimedSprite(
                                        monster_sprite->rect.center(), &(*ui_icons)["cross"],
                                        1.0f);
                                timed.add(&battle_sprites);
                                break;
                            }
                        }

                    }
                }
            }
            else if (selection_mode == SELECTMODE_ATTACKS)
            {
                const auto abilities = current_monster->monster->GetAbilities(false);
                // when monster is out of energy abilities returns empty
                if (!abilities.empty())
                {
                    selection_mode = SELECTMODE_TARGET;
                    selected_attack = abilities[indexes[SELECTMODE_ATTACKS]];
                    selection_side = GameData::GetInstance().ATTACK_DATA[selected_attack].target;
                }
                else
                {
                    selection_mode = SELECTMODE_GENERAL;
                }
            }
            else if (selection_mode == SELECTMODE_GENERAL)
            {
                if (indexes[SELECTMODE_GENERAL] == 0)
                {
                    selection_mode = SELECTMODE_ATTACKS;
                }
                else if (indexes[SELECTMODE_GENERAL] == 1)
                {
                    // select defense resumes battle
                    current_monster->monster->defending = true;
                    UpdateAllMonsters(false);
                    current_monster = nullptr;
                    selection_mode = SELECTMODE_NONE;
                    indexes[SELECTMODE_GENERAL] = 0;
                }
                else if (indexes[SELECTMODE_GENERAL] == 2)
                {
                    selection_mode = SELECTMODE_SWITCH;
                }
                else if (indexes[SELECTMODE_GENERAL] == 3)
                {
                    selection_mode = SELECTMODE_TARGET;
                    selection_side = OPPONENT;
                }
            }

            // reset all indexes after an action
            // the tutorial uses this, but we deal in limiter or checking vector<> sizes
            // for (auto &[mode, value]: indexes)
            // {
            //     indexes[mode] = 0;
            // }
        }
        if (IsKeyPressed(rl::KEY_ESCAPE))
        {
            if (selection_mode >= SELECTMODE_ATTACKS && selection_mode <= SELECTMODE_TARGET)
            {
                selection_mode = SELECTMODE_GENERAL;
            }
        }
    }
}

void Battle::ApplyAttack(
        const MonsterSprite *target_sprite, const Attack attack, float amount)
{
    // play an animation
    for (auto &at: attack_sprites_)
    {
        if (!at.active)
        {
            at = AttackSprite(
                    target_sprite->rect.center(),
                    &(*attack_frames)[GameData::GetInstance().ATTACK_DATA[attack].animation]);
            at.add(&battle_sprites);
            (*sounds)[GameData::GetInstance().NAMES_ATTACK_ANIMATION[GameData::GetInstance().
                ATTACK_DATA[attack].animation]].Play();
            break;
        }
    }

    // get correct attack damage amount (defense, element)
    // double attack if effective
    const auto attack_element = GameData::GetInstance().ATTACK_DATA[attack].element;
    const auto target_element = target_sprite->monster->element;
    if ((attack_element == ELEMENT_FIRE && target_element == ELEMENT_FIRE) ||
        (attack_element == ELEMENT_WATER && target_element == ELEMENT_FIRE) ||
        (attack_element == ELEMENT_PLANT && target_element == ELEMENT_WATER))
    {
        amount *= 2;
    }
    // half attack if not effective
    if ((attack_element == ELEMENT_FIRE && target_element == ELEMENT_WATER) ||
        (attack_element == ELEMENT_WATER && target_element == ELEMENT_PLANT) ||
        (attack_element == ELEMENT_PLANT && target_element == ELEMENT_FIRE))
    {
        amount *= 0.5f;
    }

    // update defense based on monster level and base stats
    // it will lower the amount above
    auto target_defense = 1 - target_sprite->monster->GetStat("defense") / 2000;
    if (target_sprite->monster->defending)
    {
        target_defense -= 0.2f;
    }
    target_defense = rl::Clamp(target_defense, 0, 1);

    // update monster health
    target_sprite->monster->health -= amount * target_defense;
    CheckDeath();

    // resume
    UpdateAllMonsters(false);
}

void Battle::CheckDeath()
{
    CheckDeathGroup(&player_sprites, PLAYER);
    CheckDeathGroup(&opponent_sprites, OPPONENT);
}

void Battle::CheckDeathGroup(rg::sprite::Group *group, const SelectionSide side)
{
    for (auto *sprite: group->Sprites())
    {
        const auto monster_sprite = dynamic_cast<MonsterSprite *>(sprite);
        if (monster_sprite->monster->health <= 0)
        {
            Monster *newMonster = nullptr;
            int newIndex = 0, newPosIndex = 0; // new monster
            if (side == PLAYER)
            {
                // monsters in the battle
                std::vector<std::pair<const int, Monster *>> active_monsters;
                for (auto *player_sprite: player_sprites.Sprites())
                {
                    const auto player_monster_sprite =
                            dynamic_cast<MonsterSprite *>(player_sprite);
                    active_monsters.emplace_back(
                            player_monster_sprite->index, player_monster_sprite->monster);
                }

                // monsters with health and not in battle
                std::vector<std::pair<const int, Monster *>> available_monsters_option;
                for (auto &[i, monster]: *player_monsters)
                {
                    std::pair<const int, Monster *> pair_monster = {i, &monster};
                    if (monster.health > 0 &&
                        std::ranges::find(active_monsters, pair_monster) == active_monsters.end()
                    )
                    {
                        available_monsters_option.emplace_back(i, &monster);
                        break;
                    }
                }

                // if there are monsters to add to battle
                if (!available_monsters_option.empty())
                {
                    newMonster = available_monsters_option[0].second;
                    newIndex = available_monsters_option[0].first;
                    newPosIndex = monster_sprite->pos_index;
                }
            }
            else
            {
                // check if opponent has more monsters
                if (!opponent_extra_monsters.empty())
                {
                    newMonster = opponent_extra_monsters.begin()->second;
                    newIndex = monster_sprite->index;
                    newPosIndex = monster_sprite->pos_index;
                    opponent_extra_monsters.erase(opponent_extra_monsters.begin());
                }
                else
                {
                    indexes[SELECTMODE_TARGET] = 0;
                }
                // increase XP
                if (!player_sprites.Sprites().empty())
                {
                    const int xp_amount =
                            monster_sprite->monster->level * 100 / player_sprites.Sprites().size();
                    for (auto *player_sprite: player_sprites.Sprites())
                    {
                        dynamic_cast<MonsterSprite *>(player_sprite)
                                ->monster->UpdateXP(xp_amount);
                    }
                }
            }
            monster_sprite->DelayedKill(newMonster, newIndex, newPosIndex, side);
            selection_mode = SELECTMODE_NONE;
        }
    }
}

void Battle::CheckEndBattle()
{
    // opponents have been defeated
    if (opponent_sprites.Sprites().empty() && !battle_over)
    {
        battle_over = true;
        for (auto &monster: *player_monsters | std::views::values)
        {
            monster.initiative = 0;
        }
        endBattle(character);
    }
    // player has been defeated
    if (player_sprites.Sprites().empty())
    {
        rg::Quit();
    }
}

void Battle::DrawUi()
{
    if (current_monster)
    {
        if (selection_mode == SELECTMODE_GENERAL)
        {
            DrawGeneral();
        }
        else if (selection_mode == SELECTMODE_ATTACKS)
        {
            DrawAttacks();
        }
        else if (selection_mode == SELECTMODE_SWITCH)
        {
            DrawSwitch();
        }
    }
}

void Battle::DrawGeneral()
{
    int index = 0;
    for (auto &[pos, icon]: Settings::GetInstance().BATTLE_CHOICES["full"] | std::views::values)
    {
        rg::Surface *surf;
        if (index == indexes[SELECTMODE_GENERAL])
        {
            surf = &(*ui_icons)[icon + "_highlight"];
        }
        else
        {
            surf = &(*ui_icons)[icon + "_gray"];
        }
        const auto rect =
                surf->GetRect().center(current_monster->rect.midright() + pos);
        display_surface->Blit(surf, rect);
        ++index;
    }
}

void Battle::DrawAttacks()
{
    // data
    const auto abilities = current_monster->monster->GetAbilities(false);
    constexpr float width = 150;
    constexpr float height = 200;
    constexpr int visible_attacks = 4;
    constexpr float item_height = height / visible_attacks;
    int v_offset;
    if (indexes[SELECTMODE_ATTACKS] < visible_attacks)
    {
        v_offset = 0;
    }
    else
    {
        v_offset = -(indexes[SELECTMODE_ATTACKS] - visible_attacks + 1) * float(item_height);
    }

    // bg
    const auto bg_rect = rg::Rect{0, 0, width, height}.midleft(
            current_monster->rect.midright() + rg::math::Vector2{20, 0});
    rg::draw::rect(display_surface, Settings::GetInstance().COLORS["white"], bg_rect, 0, 5);

    for (size_t index = 0; index < abilities.size(); ++index)
    {
        const bool selected = index == indexes[SELECTMODE_ATTACKS];
        rl::Color text_color;
        if (selected)
        {
            auto element = GameData::GetInstance().ATTACK_DATA[abilities[index]].element;
            if (element == ELEMENT_NORMAL)
            {
                text_color = Settings::GetInstance().COLORS["black"];
            }
            else
            {
                text_color = Settings::GetInstance().COLORS[GameData::GetInstance().
                    NAMES_ELEMENT_TYPES[element]];
            }
        }
        else
        {
            text_color = Settings::GetInstance().COLORS["light"];
        }
        auto text_surf =
                (*fonts)["regular"].render(
                        GameData::GetInstance().ATTACK_DATA[abilities[index]].name.c_str(),
                        text_color);
        auto text_rect = text_surf.GetRect().center(
                bg_rect.midtop() +
                rg::math::Vector2{0.0f, item_height / 2.0f + index * item_height + v_offset});
        auto text_bg_rect = rg::Rect{0, 0, width, item_height}.center(text_rect.center());

        // draw
        if (bg_rect.collidepoint(text_rect.center()))
        {
            if (selected)
            {
                if (text_bg_rect.collidepoint(bg_rect.topleft()))
                {
                    rg::draw::rect(
                            display_surface, Settings::GetInstance().COLORS["dark white"],
                            text_bg_rect, 0, 5, true, true,
                            false, false);
                }
                else if (text_bg_rect.collidepoint(bg_rect.midbottom() + rg::math::Vector2{0, -1}))
                {
                    rg::draw::rect(
                            display_surface, Settings::GetInstance().COLORS["dark white"],
                            text_bg_rect, 0, 5, false, false,
                            true, true);
                }
                else
                {
                    rg::draw::rect(
                            display_surface, Settings::GetInstance().COLORS["dark white"],
                            text_bg_rect);
                }
            }
            display_surface->Blit(&text_surf, text_rect);
        }
    }
}

void Battle::DrawSwitch()
{
    // data
    constexpr float width = 300;
    constexpr float height = 320;
    constexpr int visible_monsters = 4;
    constexpr float item_height = height / visible_monsters;
    int v_offset;
    if (indexes[SELECTMODE_SWITCH] < visible_monsters)
    {
        v_offset = 0;
    }
    else
    {
        v_offset = -(indexes[SELECTMODE_SWITCH] - visible_monsters + 1) * float(item_height);
    }

    // bg
    const auto bg_rect = rg::Rect{0, 0, width, height}.midleft(
            current_monster->rect.midright() + rg::math::Vector2{20, 0});
    rg::draw::rect(display_surface, Settings::GetInstance().COLORS["white"], bg_rect, 0, 5);

    // monsters
    std::vector<std::pair<int, Monster *>> active_monsters; // monsters in battle
    active_monsters.reserve(player_sprites.Sprites().size());
    for (auto *monster: player_sprites.Sprites())
    {
        const auto monster_sprite = dynamic_cast<MonsterSprite *>(monster);
        active_monsters.emplace_back(monster_sprite->index, monster_sprite->monster);
    }

    // find player's monsters that are not in battle
    available_monsters.clear();
    for (auto &[index, monster]: *player_monsters)
    {
        auto find_pair = std::make_pair(index, &monster);
        if (monster.health > 0 &&
            std::ranges::find(active_monsters, find_pair) == active_monsters.end()
        )
        {
            available_monsters[index] = &monster;
        }
    }

    int index = 0;
    for (auto *monster: available_monsters | std::views::values)
    {
        const bool selected = index == indexes[SELECTMODE_SWITCH];
        auto item_bg_rect = rg::Rect{0, 0, width, item_height}.midleft(
                rg::math::Vector2{
                        bg_rect.left(),
                        bg_rect.top() + item_height / 2 + index * item_height + v_offset});
        auto *icon_surf = &(*monster_icons)[monster->name];
        auto icon_rect = icon_surf->GetRect().midleft(
                bg_rect.topleft() +
                rg::math::Vector2{10.0f, item_height / 2.0f + index * item_height + v_offset});
        auto text_surf = (*fonts)["regular"].render(
                rl::TextFormat("%s (%d)", monster->name.c_str(), monster->level),
                selected
                    ? Settings::GetInstance().COLORS["red"]
                    : Settings::GetInstance().COLORS["black"]);
        auto text_rect = text_surf.GetRect().topleft({bg_rect.left() + 90, icon_rect.top()});

        // selection bg
        if (selected)
        {
            if (item_bg_rect.collidepoint(bg_rect.topleft()))
            {
                rg::draw::rect(
                        display_surface, Settings::GetInstance().COLORS["dark white"], item_bg_rect,
                        0, 5, true, true);
            }
            else if (item_bg_rect.collidepoint(bg_rect.midbottom() + rg::math::Vector2{0, -1}))
            {
                rg::draw::rect(
                        display_surface, Settings::GetInstance().COLORS["dark white"], item_bg_rect,
                        0, 5, false, false,
                        true, true);
            }
            else
            {
                rg::draw::rect(
                        display_surface, Settings::GetInstance().COLORS["dark white"],
                        item_bg_rect);
            }
        }

        if (bg_rect.collidepoint(item_bg_rect.center()))
        {
            display_surface->Blit(icon_surf, icon_rect);
            display_surface->Blit(&text_surf, text_rect);

            auto health_rect =
                    rg::Rect{text_rect.bottomleft() + rg::math::Vector2{0, 4}, {100.0f, 4.0f}};
            auto energy_rect =
                    rg::Rect{health_rect.bottomleft() + rg::math::Vector2{0, 2}, {80, 4}};
            rg::draw::bar(
                    display_surface, health_rect, monster->health, monster->GetStat("max_health"),
                    Settings::GetInstance().COLORS["red"], Settings::GetInstance().COLORS["black"]);
            rg::draw::bar(
                    display_surface, energy_rect, monster->energy, monster->GetStat("max_energy"),
                    Settings::GetInstance().COLORS["blue"],
                    Settings::GetInstance().COLORS["black"]);
        }

        ++index;
    }
}

void Battle::UpdateTimers()
{
    for (auto &timer: timers | std::views::values)
    {
        timer.Update();
    }
}

void Battle::OpponentAttack()
{
    if (!current_monster)
    {
        return;
    }
    const auto abilities = current_monster->monster->GetAbilities();
    // if monster is out of energy, abilities is empty
    if (abilities.empty())
    {
        return;
    }
    const auto random_ability_index = rl::GetRandomValue(0, abilities.size() - 1);
    const auto ability = abilities[random_ability_index];

    const auto side = GameData::GetInstance().ATTACK_DATA[ability].target;
    // side of the attack = PLAYER - attack same team (healing/defense) | OPPONENT - attack the
    // other team
    if (side == PLAYER)
    {
        SetAttack(&opponent_sprites, ability);
    }
    else
    {
        SetAttack(&player_sprites, ability);
    }
}

void Battle::SetAttack(rg::sprite::Group *group, const Attack ability) const
{
    const auto sprites = group->Sprites();

    if (!sprites.empty())
    {
        const auto random_target_index = rl::GetRandomValue(0, sprites.size() - 1);
        auto *sprite = sprites[random_target_index];
        const auto random_target = dynamic_cast<MonsterSprite *>(sprite);
        current_monster->ActivateAttack(random_target, ability);
    }
}
