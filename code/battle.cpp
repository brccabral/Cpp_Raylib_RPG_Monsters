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
    monsters_paused = false;
    battle_monster.resize(6);
    timed_sprites_.resize(6);
    attack_sprites_.resize(6);
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
    // remove from `monster_data` opponents monsters that were created as this will serve
    // as container to add new monsters in battle after a defeat
    // for (auto it = opponent_monsters->begin(); it != opponent_monsters->end();)
    // {
    //     if (std::find(added_opponents.begin(), added_opponents.end(), it->first) !=
    //         added_opponents.end())
    //     {
    //         it = opponent_monsters->erase(it);
    //     }
    //     else
    //     {
    //         ++it;
    //     }
    // }
}

void Battle::CreateMonster(
        Monster *monster, int index, int pos_index, SelectionSide entity)
{
    auto &frames = (*monster_frames)[monster->name];
    auto &outlines = (*outline_frames)[monster->name];
    rg::math::Vector2 pos;
    auto groups = std::vector<rg::sprite::Group *>{};

    if (entity == PLAYER)
    {
        pos = BATTLE_POSITIONS["left"][pos_index];
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
        pos = BATTLE_POSITIONS["right"][pos_index];
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

void Battle::CheckActiveGroup(const rg::sprite::Group *group, const SelectionSide side)
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
    for (const auto &sprite: player_sprites.Sprites())
    {
        const auto monster_sprite = dynamic_cast<MonsterSprite *>(sprite);
        monster_sprite->monster->paused = do_pause;
    }
    for (const auto &sprite: opponent_sprites.Sprites())
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
                limiter = BATTLE_CHOICES["full"].size();
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
                const auto *sprite_group =
                        selection_side == PLAYER ? &player_sprites : &opponent_sprites;
                // when a monster gets defeated, the group may change, but the "pos_index" won't
                // create a list ordered by "pos_index"
                auto ordered_pos = sprite_group->Sprites();
                std::sort(
                        ordered_pos.begin(), ordered_pos.end(),
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
                        monster_sprite->entity = PLAYER;
                        // when deleting, set to PLAYER to not delete `Monster *`
                        (*player_monsters)[player_monsters->size()] = std::move(
                                *monster_sprite->monster);
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
                    selection_side = ATTACK_DATA[selected_attack].target;
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
                    target_sprite->rect.center(), &(*attack_frames)[ATTACK_DATA[attack].animation]);
            at.add(&battle_sprites);
            (*sounds)[NAMES_ATTACK_ANIMATION[ATTACK_DATA[attack].animation]].Play();
            break;
        }
    }

    // get correct attack damage amount (defense, element)
    // double attack if effective
    const auto attack_element = ATTACK_DATA[attack].element;
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

void Battle::CheckDeathGroup(const rg::sprite::Group *group, const SelectionSide side)
{
    for (const auto &sprite: group->Sprites())
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
                        std::find(
                                available_monsters_option.begin(), active_monsters.end(),
                                pair_monster) ==
                        active_monsters.end())
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
                if (!opponent_monsters->empty())
                {
                    newMonster = &opponent_monsters->begin()->second;
                    newIndex = monster_sprite->index;
                    newPosIndex = monster_sprite->pos_index;
                    opponent_monsters->erase(opponent_monsters->begin());
                }
                else
                {
                    indexes[SELECTMODE_TARGET] = 0;
                }
                // increase XP
                auto p_sprites = player_sprites.Sprites();
                if (!p_sprites.empty())
                {
                    const int xp_amount = monster_sprite->monster->level * 100 / p_sprites.size();
                    for (const auto &player_sprite: p_sprites)
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
        for (auto &[i, monster]: *player_monsters)
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
    for (auto &[option, battle_choice]: BATTLE_CHOICES["full"])
    {
        rg::Surface *surf;
        if (index == indexes[SELECTMODE_GENERAL])
        {
            surf = &(*ui_icons)[battle_choice.icon + "_highlight"];
        }
        else
        {
            surf = &(*ui_icons)[battle_choice.icon + "_gray"];
        }
        const auto rect =
                surf->GetRect().center(current_monster->rect.midright() + battle_choice.pos);
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
    rg::draw::rect(display_surface, COLORS["white"], bg_rect, 0, 5);

    for (int index = 0; index < (int) abilities.size(); ++index)
    {
        const bool selected = index == indexes[SELECTMODE_ATTACKS];
        rl::Color text_color;
        if (selected)
        {
            auto element = ATTACK_DATA[abilities[index]].element;
            if (element == ELEMENT_NORMAL)
            {
                text_color = COLORS["black"];
            }
            else
            {
                text_color = COLORS[NAMES_ELEMENT_TYPES[element]];
            }
        }
        else
        {
            text_color = COLORS["light"];
        }
        auto text_surf =
                (*fonts)["regular"].render(ATTACK_DATA[abilities[index]].name.c_str(), text_color);
        auto text_rect = text_surf.GetRect().center(
                bg_rect.midtop() +
                rg::math::Vector2{0, item_height / 2 + index * item_height + v_offset});
        auto text_bg_rect = rg::Rect{0, 0, width, item_height}.center(text_rect.center());

        // draw
        if (bg_rect.collidepoint(text_rect.center()))
        {
            if (selected)
            {
                if (text_bg_rect.collidepoint(bg_rect.topleft()))
                {
                    rg::draw::rect(
                            display_surface, COLORS["dark white"], text_bg_rect, 0, 5, true, true,
                            false, false);
                }
                else if (text_bg_rect.collidepoint(bg_rect.midbottom() + rg::math::Vector2{0, -1}))
                {
                    rg::draw::rect(
                            display_surface, COLORS["dark white"], text_bg_rect, 0, 5, false, false,
                            true, true);
                }
                else
                {
                    rg::draw::rect(display_surface, COLORS["dark white"], text_bg_rect);
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
    rg::draw::rect(display_surface, COLORS["white"], bg_rect, 0, 5);

    // monsters
    std::vector<std::pair<int, Monster *>> active_monsters; // monsters in battle
    active_monsters.reserve(player_sprites.Sprites().size());
    for (auto &monster: player_sprites.Sprites())
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
            std::find(active_monsters.begin(), active_monsters.end(), find_pair) ==
            active_monsters.end())
        {
            available_monsters[index] = &monster;
        }
    }

    int index = 0;
    for (auto &[i, monster]: available_monsters)
    {
        const bool selected = index == indexes[SELECTMODE_SWITCH];
        auto item_bg_rect = rg::Rect{0, 0, width, item_height}.midleft(
                rg::math::Vector2{
                        bg_rect.left(),
                        bg_rect.top() + item_height / 2 + index * item_height + v_offset});
        auto *icon_surf = &(*monster_icons)[monster->name];
        auto icon_rect = icon_surf->GetRect().midleft(
                bg_rect.topleft() +
                rg::math::Vector2{10, item_height / 2 + index * item_height + v_offset});
        auto text_surf = (*fonts)["regular"].render(
                rl::TextFormat("%s (%d)", monster->name.c_str(), monster->level),
                selected ? COLORS["red"] : COLORS["black"]);
        auto text_rect = text_surf.GetRect().topleft({bg_rect.left() + 90, icon_rect.top()});

        // selection bg
        if (selected)
        {
            if (item_bg_rect.collidepoint(bg_rect.topleft()))
            {
                rg::draw::rect(
                        display_surface, COLORS["dark white"], item_bg_rect, 0, 5, true, true);
            }
            else if (item_bg_rect.collidepoint(bg_rect.midbottom() + rg::math::Vector2{0, -1}))
            {
                rg::draw::rect(
                        display_surface, COLORS["dark white"], item_bg_rect, 0, 5, false, false,
                        true, true);
            }
            else
            {
                rg::draw::rect(display_surface, COLORS["dark white"], item_bg_rect);
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
                    COLORS["red"], COLORS["black"]);
            rg::draw::bar(
                    display_surface, energy_rect, monster->energy, monster->GetStat("max_energy"),
                    COLORS["blue"], COLORS["black"]);
        }

        ++index;
    }
}

void Battle::UpdateTimers()
{
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }
}

void Battle::OpponentAttack() const
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

    const auto side = ATTACK_DATA[ability].target;
    // side of the attack = PLAYER - attack same team (healing/defense) | OPPONENT - attack the
    // other team
    const std::vector<rg::sprite::Sprite *> *sprites;
    if (side == PLAYER)
    {
        sprites = &opponent_sprites.Sprites();
    }
    else
    {
        sprites = &player_sprites.Sprites();
    }

    if (!sprites->empty())
    {
        const auto random_target_index = rl::GetRandomValue(0, sprites->size() - 1);
        const auto random_target =
                dynamic_cast<MonsterSprite *>((*sprites)[random_target_index]);
        current_monster->ActivateAttack(random_target, ability);
    }
}
