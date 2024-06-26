#include <cstring>
#include <iostream>
#include <utility>
#include "battle.h"
#include "groups.h"

#include <list>


Battle::Battle(
        const std::map<int, Monster *> &player_monsters,
        const std::map<int, Monster *> &opponent_monsters,
        const std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>>
                &monsters_frames,
        const std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>>
                &outline_frames,
        const std::map<std::string, Texture2D> &ui_frms, const Texture2D &bg_surf,
        const std::map<std::string, Texture2D> &monster_icons,
        const std::map<AttackAnimation, std::vector<TiledTexture>> &attack_animation_frms,
        const std::map<std::string, Font> &fonts)
    : bg_surf(bg_surf), monsters_frames(monsters_frames), outline_frames(outline_frames),
      ui_frames(ui_frms), fonts(fonts),
      monster_data({{PLAYER, player_monsters}, {OPPONENT, opponent_monsters}}),
      monster_icons(monster_icons), attack_animation_frames(attack_animation_frms)
{
    battle_sprites = new BattleSprites();
    player_sprites = new SpriteGroup();
    opponent_sprites = new SpriteGroup();

    indexes[SELECTMODE_GENERAL] = 0;
    indexes[SELECTMODE_MONSTER] = 0;
    indexes[SELECTMODE_ATTACKS] = 0;
    indexes[SELECTMODE_SWITCH] = 0;
    indexes[SELECTMODE_TARGET] = 0;

    timers["opponent_delay"] = new Timer(0.6f, false, false, [this] { OpponentAttack(); });

    Setup();
}

Battle::~Battle()
{
    delete battle_sprites;
    delete player_sprites;
    delete opponent_sprites;
    for (auto &[key, timer]: timers)
    {
        delete timer;
    }
}

void Battle::Update(const double dt)
{
    CreateNewMonsters(); // create new monsters outside the Update loop
    Input();
    UpdateTimers();
    // Update Sprites before drawing into `display_surface`, as
    // some sprites open renderes to draw into
    battle_sprites->Update(dt);
    CheckActive();

    BeginTextureModeC(display_surface, BLACK);
    DrawTexture(bg_surf, 0, 0, WHITE);
    battle_sprites->Draw(
            current_monster, selection_side, selection_mode, indexes[SELECTMODE_TARGET],
            player_sprites, opponent_sprites);
    DrawUi();
    EndTextureModeSafe();
}

void Battle::Setup()
{
    std::vector<int> added_opponents;

    for (auto &[entity, monsters]: monster_data)
    {
        for (auto &[index, monster]: monsters)
        {
            if (index <= 2)
            {
                AddNewMonster(monster, index, index, entity);
                if (entity == OPPONENT)
                {
                    added_opponents.push_back(index);
                }
            }
        }
    }
    // remove from `monster_data` opponents monsters that were createdas this will serve
    // as container to add new monsters in battle after a defeat
    for (auto it = monster_data[OPPONENT].begin(); it != monster_data[OPPONENT].end();)
    {
        if (std::find(added_opponents.begin(), added_opponents.end(), it->first) !=
            added_opponents.end())
        {
            it = monster_data[OPPONENT].erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Battle::AddNewMonster(Monster *monster, int index, int pos_index, SelectionSide entity)
{
    newMonstersData.emplace_back(monster, index, pos_index, entity);
}

// create new monsters outside the Update loop
void Battle::CreateNewMonsters()
{
    if (!newMonstersData.empty())
    {
        for (auto monsterData: newMonstersData)
        {
            CreateMonster(
                    std::get<0>(monsterData), std::get<1>(monsterData), std::get<2>(monsterData),
                    std::get<3>(monsterData));
        }
        newMonstersData.clear();
    }
}

void Battle::CreateMonster(
        Monster *monster, const int index, const int pos_index, const SelectionSide entity)
{
    const auto frames = monsters_frames[monster->name];
    const auto outlines = outline_frames[monster->name];
    MonsterSprite *monster_sprite;
    MonsterNameSprite *name_sprite;
    MonsterOutlineSprite *outline_sprite;
    Vector2 name_pos;
    Vector2 level_pos;
    if (entity == PLAYER)
    {
        monster_sprite = new MonsterSprite(
                BATTLE_POSITIONS["left"][pos_index], frames, {battle_sprites, player_sprites},
                monster, index, pos_index, entity, this);
        monster_sprite->FlipH();
        name_pos = Vector2Add(GetRectMidLeft(monster_sprite->rect), {-40, -70});
        name_sprite =
                new MonsterNameSprite(name_pos, monster_sprite, {battle_sprites}, fonts["regular"]);
        level_pos = GetRectBottomLeft(name_sprite->rect);
        outline_sprite = new MonsterOutlineSprite(monster_sprite, {battle_sprites}, outlines);
        outline_sprite->FlipH();
    }
    else
    {
        monster_sprite = new MonsterSprite(
                BATTLE_POSITIONS["right"][pos_index], frames, {battle_sprites, opponent_sprites},
                monster, index, pos_index, entity, this);
        name_pos = Vector2Add(GetRectMidRight(monster_sprite->rect), {-60, -70});
        name_sprite =
                new MonsterNameSprite(name_pos, monster_sprite, {battle_sprites}, fonts["regular"]);
        level_pos = GetRectBottomRight(name_sprite->rect);
        outline_sprite = new MonsterOutlineSprite(monster_sprite, {battle_sprites}, outlines);
    }
    auto *level_sprite = new MonsterLevelSprite(
            entity, level_pos, monster_sprite, {battle_sprites}, fonts["small"]);
    auto *stats_sprite = new MonsterStatsSprite(
            Vector2Add(GetRectMidBottom(monster_sprite->rect), {0.20}), monster_sprite, {150, 48},
            {battle_sprites}, fonts["small"]);

    //  set the sub sprites to be `Kill()`ed together
    monster_sprite->SetNameSprite(name_sprite);
    monster_sprite->SetLevelSprite(level_sprite);
    monster_sprite->SetStatsSprite(stats_sprite);
    monster_sprite->SetOutlineSprite(outline_sprite);
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
                limiter = selection_side == OPPONENT ? opponent_sprites->sprites.size()
                                                     : player_sprites->sprites.size();
                break;
            }
            default:
                break;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            indexes[selection_mode] =
                    limiter ? ((indexes[selection_mode] + 1) % limiter + limiter) % limiter : 0;
        }
        if (IsKeyPressed(KEY_UP))
        {
            indexes[selection_mode] =
                    limiter ? ((indexes[selection_mode] - 1) % limiter + limiter) % limiter : 0;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            if (selection_mode == SELECTMODE_SWITCH)
            {
                if (!available_monsters.empty())
                {
                    const int newIndex = indexes[SELECTMODE_SWITCH];
                    const int newPosIndex = current_monster->pos_index;
                    Monster *newMonster = available_monsters[newIndex];
                    current_monster->Kill();
                    AddNewMonster(newMonster, newIndex, newPosIndex, PLAYER);
                    selection_mode = SELECTMODE_NONE;
                    UpdateAllMonsters(false);
                }
            }
            else if (selection_mode == SELECTMODE_TARGET)
            {
                auto *sprite_group = selection_side == OPPONENT ? opponent_sprites : player_sprites;
                // when a monster gets defeated, the group may change, but the "pos_index" won't
                // create a list ordered by "pos_index"
                std::vector<MonsterSprite *> ordered_pos;
                for (auto *sprite: sprite_group->sprites)
                {
                    ordered_pos.push_back((MonsterSprite *) sprite);
                }
                std::sort(
                        ordered_pos.begin(), ordered_pos.end(),
                        [](const MonsterSprite *a, const MonsterSprite *b)
                        { return a->pos_index < b->pos_index; });

                auto *monster_sprite = ordered_pos[indexes[SELECTMODE_TARGET]];
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
                        monster_data[PLAYER][monster_data[PLAYER].size()] = monster_sprite->monster;
                        monster_sprite->DelayedKill(
                                nullptr, 0, 0,
                                OPPONENT); // kills the MonsterSprite*, not the Monter*
                        UpdateAllMonsters(false);
                    }
                    else
                    {
                        const RectangleU cross_rect = {
                                0, 0, (float) ui_frames["cross"].width,
                                (float) ui_frames["cross"].height};
                        new TimedSprite(
                                GetRectCenter(monster_sprite->rect),
                                {&ui_frames["cross"], cross_rect}, {battle_sprites}, 1.0f);
                    }
                }
            }
            else if (selection_mode == SELECTMODE_ATTACKS)
            {
                selection_mode = SELECTMODE_TARGET;
                selected_attack =
                        current_monster->monster->GetAbilities(false)[indexes[SELECTMODE_ATTACKS]];
                selection_side = ATTACK_DATA[selected_attack].target;
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
            // for (auto &[mode, value]: indexes)
            // {
            //     indexes[mode] = 0;
            // }
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (selection_mode >= SELECTMODE_ATTACKS && selection_mode <= SELECTMODE_TARGET)
            {
                selection_mode = SELECTMODE_GENERAL;
            }
        }
    }
}

void Battle::CheckActive()
{
    CheckActiveGroup(player_sprites, PLAYER);
    CheckActiveGroup(opponent_sprites, OPPONENT);
}

void Battle::CheckActiveGroup(const SpriteGroup *group, const SelectionSide side)
{
    for (const auto *sprite: group->sprites)
    {
        if (((MonsterSprite *) sprite)->monster->initiative >= 100)
        {
            UpdateAllMonsters(true);
            ((MonsterSprite *) sprite)->monster->initiative = 0;
            ((MonsterSprite *) sprite)->SetHighlight(true);
            current_monster = ((MonsterSprite *) sprite);
            if (side == PLAYER)
            {
                selection_mode = SELECTMODE_GENERAL;
                selection_side = PLAYER;
            }
            else
            {
                timers["opponent_delay"]->Activate();
            }
        }
    }
}

void Battle::UpdateAllMonsters(const bool do_pause) const
{
    for (const auto *sprite: player_sprites->sprites)
    {
        ((MonsterSprite *) sprite)->monster->paused = do_pause;
    }
    for (const auto *sprite: opponent_sprites->sprites)
    {
        ((MonsterSprite *) sprite)->monster->paused = do_pause;
    }
}

void Battle::ApplyAttack(const MonsterSprite *target_sprite, const Attack attack, float amount)
{
    // play an animation
    new AttackSprite(
            GetRectCenter(target_sprite->rect),
            attack_animation_frames[ATTACK_DATA[attack].animation], {battle_sprites});

    // get correct attack damage amount
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
    target_defense = Clamp(target_defense, 0, 1);

    // update monster health
    target_sprite->monster->health -= amount * target_defense;
    CheckDeath();

    // resume
    UpdateAllMonsters(false);
}

void Battle::CheckDeath()
{
    CheckDeathGroup(player_sprites, PLAYER);
    CheckDeathGroup(opponent_sprites, OPPONENT);
}

void Battle::CheckDeathGroup(const SpriteGroup *group, const SelectionSide side)
{
    for (auto *sprite: group->sprites)
    {
        const auto monster_sprite = (MonsterSprite *) sprite;
        if (monster_sprite->monster->health <= 0)
        {
            Monster *newMonster = nullptr;
            int newIndex = 0, newPosIndex = 0; // new monster
            if (side == PLAYER)
            {
                // monsters in the battle
                std::vector<std::pair<const int, Monster *>> active_monsters;
                for (const auto player_monster_sprite: player_sprites->sprites)
                {
                    active_monsters.emplace_back(
                            ((MonsterSprite *) player_monster_sprite)->index,
                            ((MonsterSprite *) player_monster_sprite)->monster);
                }

                // monsters with health and not in battle
                std::vector<std::pair<const int, Monster *>> available_monsters;
                for (auto &player_pair: monster_data[PLAYER])
                {
                    if (player_pair.second->health > 0 &&
                        std::find(active_monsters.begin(), active_monsters.end(), player_pair) ==
                                active_monsters.end())
                    {
                        available_monsters.emplace_back(player_pair);
                        break;
                    }
                }

                // if there are monsters to add to battle
                if (!available_monsters.empty())
                {
                    newMonster = available_monsters[0].second;
                    newIndex = available_monsters[0].first;
                    newPosIndex = monster_sprite->pos_index;
                }
            }
            else
            {
                // check if opponent has more monsters
                if (!monster_data[OPPONENT].empty())
                {
                    newMonster = monster_data[OPPONENT].begin()->second;
                    newIndex = monster_sprite->index;
                    newPosIndex = monster_sprite->pos_index;
                    monster_data[OPPONENT].erase(monster_data[OPPONENT].begin());
                }
                // increase XP
                const int xp_amount =
                        monster_sprite->monster->level * 100 / player_sprites->sprites.size();
                for (auto *player_sprite: player_sprites->sprites)
                {
                    ((MonsterSprite *) player_sprite)->monster->UpdateXP(xp_amount);
                }
            }
            monster_sprite->DelayedKill(newMonster, newIndex, newPosIndex, side);
        }
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
        Texture2D texture;
        if (index == indexes[SELECTMODE_GENERAL])
        {
            texture = ui_frames[battle_choice.icon + "_highlight"];
        }
        else
        {
            texture = TextureColorGrayscale(&ui_frames[battle_choice.icon]);
        }
        auto rect = current_monster->rect;
        RectToCenter(rect, GetRectMidRight(rect));
        DrawTextureV(texture, Vector2Add(rect.pos, battle_choice.pos), WHITE);
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
        v_offset = -(indexes[SELECTMODE_ATTACKS] - visible_attacks + 1) * item_height;
    }

    // bg
    RectangleU bg_rect = {0, 0, width, height};
    RectToMidLeft(bg_rect, Vector2Add(GetRectMidRight(current_monster->rect), {20, 0}));
    DrawRectangleRounded(bg_rect.rectangle, 0.1f, 10, COLORS["white"]);

    for (int index = 0; index < abilities.size(); ++index)
    {
        const bool selected = index == indexes[SELECTMODE_ATTACKS];
        Color text_color;
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
        // rect
        const RectangleU text_rect = {
                bg_rect.x, bg_rect.y + index * item_height + v_offset, bg_rect.width, item_height};
        if (CheckCollisionPointRec(GetRectCenter(text_rect), bg_rect.rectangle))
        {
            if (selected)
            {
                if (CheckCollisionPointRec(GetRectTopLeft(bg_rect), text_rect.rectangle))
                {
                    DrawRectangleRoundedEx(
                            text_rect, 0.2f, 10, COLORS["dark white"], true, true, false, false);
                }
                else if (CheckCollisionPointRec(
                                 Vector2Add(GetRectMidBottom(bg_rect), {0, -1}),
                                 text_rect.rectangle))
                {
                    DrawRectangleRoundedEx(
                            text_rect, 0.2f, 10, COLORS["dark white"], false, false, true, true);
                }
                else
                {
                    DrawRectangleRec(text_rect.rectangle, COLORS["dark white"]);
                }
            }
            DrawCenteredTextEx(
                    fonts["regular"], ATTACK_DATA[abilities[index]].name.c_str(), text_rect,
                    text_color);
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
        v_offset = -(indexes[SELECTMODE_SWITCH] - visible_monsters + 1) * item_height;
    }

    // bg
    RectangleU bg_rect = {0, 0, width, height};
    RectToMidLeft(bg_rect, Vector2Add(GetRectMidRight(current_monster->rect), {20, 0}));
    DrawRectangleRounded(bg_rect.rectangle, 0.1f, 10, COLORS["white"]);

    // monsters
    std::vector<Monster *> active_monsters;
    for (const auto monster_sprite: player_sprites->sprites)
    {
        active_monsters.push_back(((MonsterSprite *) monster_sprite)->monster);
    }
    available_monsters.clear();
    for (auto &[monster_index, monster]: monster_data[PLAYER])
    {
        if (monster->health > 0 &&
            // if not found in active
            std::find(active_monsters.begin(), active_monsters.end(), monster) ==
                    active_monsters.end())
        {
            available_monsters.push_back(monster);
        }
    }
    for (int index = 0; index < available_monsters.size(); ++index)
    {
        Monster *monster = available_monsters[index];
        const bool selected = index == indexes[SELECTMODE_SWITCH];
        RectangleU item_bg_rect = {0, 0, width, item_height};
        RectToMidLeft(
                item_bg_rect,
                {bg_rect.x, bg_rect.y + item_height / 2 + index * item_height + v_offset});
        if (CheckCollisionPointRec(GetRectCenter(item_bg_rect), bg_rect.rectangle))
        {
            const Texture2D icon_texture = monster_icons[monster->name];
            RectangleU icon_rect = {0, 0, (float) icon_texture.width, (float) icon_texture.height};
            RectToMidLeft(
                    icon_rect, Vector2Add(
                                       GetRectTopLeft(bg_rect),
                                       {10, item_height / 2 + index * item_height + v_offset}));
            const Color text_color = selected ? COLORS["red"] : COLORS["black"];
            if (selected)
            {
                if (CheckCollisionPointRec(GetRectTopLeft(bg_rect), item_bg_rect.rectangle))
                {
                    DrawRectangleRoundedEx(
                            item_bg_rect, 0.4f, 10, COLORS["dark white"], true, true, false, false);
                }
                else if (CheckCollisionPointRec(
                                 Vector2Add(GetRectMidBottom(bg_rect), {0, -1}),
                                 item_bg_rect.rectangle))
                {
                    DrawRectangleRoundedEx(
                            item_bg_rect, 0.4f, 10, COLORS["dark white"], false, false, true, true);
                }
                else
                {
                    DrawRectangleRec(item_bg_rect.rectangle, COLORS["dark white"]);
                }
            }
            DrawTextEx(
                    fonts["regular"], TextFormat("%s (%i)", monster->name.c_str(), monster->level),
                    {bg_rect.x + 90, icon_rect.y}, fonts["regular"].baseSize, 1, text_color);
            DrawTextureV(icon_texture, icon_rect.pos, WHITE);

            RectangleU health_rectangle = {
                    bg_rect.x + 90, icon_rect.y + fonts["regular"].baseSize + 4, 100, 4};
            RectangleU energy_rectangle = {
                    Vector2Add(GetRectBottomLeft(health_rectangle), {0, 2}), {80, 4}};
            DrawBar(health_rectangle, monster->health, monster->GetStat("max_health"),
                    COLORS["red"], COLORS["black"]);
            DrawBar(energy_rectangle, monster->energy, monster->GetStat("max_energy"),
                    COLORS["blue"], COLORS["black"]);
        }
    }
}

void Battle::UpdateTimers()
{
    for (auto &[key, timer]: timers)
    {
        timer->Update();
    }
}

void Battle::OpponentAttack()
{
    const auto abilities = current_monster->monster->GetAbilities();
    const auto random_ability_index = GetRandomValue(0, abilities.size());
    const auto ability = abilities[random_ability_index];

    const auto side = ATTACK_DATA[ability].target;
    // PLAYER - attack same team (healing/defense) | OPPONENT - attack the other team
    MonsterSprite *random_target;
    if (side == PLAYER)
    {
        const auto random_target_index = GetRandomValue(0, opponent_sprites->sprites.size() - 1);
        random_target = (MonsterSprite *) opponent_sprites->sprites[random_target_index];
    }
    else
    {
        const auto random_target_index = GetRandomValue(0, player_sprites->sprites.size() - 1);
        random_target = (MonsterSprite *) player_sprites->sprites[random_target_index];
    }
    current_monster->ActivateAttack(random_target, ability);
}
