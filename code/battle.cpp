#include <cstring>
#include <iostream>
#include <utility>
#include "battle.h"
#include "groups.h"


Battle::Battle(
        const std::vector<Monster *> &player_monsters,
        const std::vector<Monster *> &opponent_monsters,
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

    Setup();
}

Battle::~Battle()
{
    delete battle_sprites;
    delete player_sprites;
    delete opponent_sprites;
}

void Battle::Update(const double dt)
{
    Input();
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
    for (auto &[entity, monsters]: monster_data)
    {
        for (int index = 0; index <= 2; ++index)
        {
            CreateMonster(monsters[index], index, index, entity);
        }
    }
}

void Battle::CreateMonster(
        Monster *monster, const int index, const int pos_index, const SelectionSide entity)
{
    const auto frames = monsters_frames[monster->name];
    const auto outlines = outline_frames[monster->name];
    Vector2 pos;
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
            indexes[selection_mode] = ((indexes[selection_mode] + 1) % limiter + limiter) % limiter;
        }
        if (IsKeyPressed(KEY_UP))
        {
            indexes[selection_mode] = ((indexes[selection_mode] - 1) % limiter + limiter) % limiter;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            if (selection_mode == SELECTMODE_TARGET)
            {
                auto *sprite_group = selection_side == OPPONENT ? opponent_sprites : player_sprites;
                // when a monster gets defeated, the group may change, but the "pos_index" won't
                // create a list with just the "pos_index"
                std::vector<int> sprites_indexes;
                for (auto *sprite: sprite_group->sprites)
                {
                    sprites_indexes.push_back(((MonsterSprite *) sprite)->pos_index);
                }
                auto *monster_sprite =
                        (MonsterSprite *)
                                sprite_group->sprites[sprites_indexes[indexes[SELECTMODE_TARGET]]];
                if (selected_attack)
                {
                    current_monster->ActivateAttack(monster_sprite, selected_attack);
                    selected_attack = ATTACK_NONE;
                    current_monster = nullptr;
                    selection_mode = SELECTMODE_NONE;
                }
            }
            if (selection_mode == SELECTMODE_ATTACKS)
            {
                selection_mode = SELECTMODE_TARGET;
                selected_attack =
                        current_monster->monster->GetAbilities(false)[indexes[SELECTMODE_ATTACKS]];
                selection_side = ATTACK_DATA[selected_attack].target;
            }
            if (selection_mode == SELECTMODE_GENERAL)
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
                {}
            }
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
    CheckActiveGroup(player_sprites);
    CheckActiveGroup(opponent_sprites);
}

void Battle::CheckActiveGroup(const SpriteGroup *group)
{
    for (const auto *sprite: group->sprites)
    {
        if (((MonsterSprite *) sprite)->monster->initiative >= 100)
        {
            UpdateAllMonsters(true);
            ((MonsterSprite *) sprite)->monster->initiative = 0;
            ((MonsterSprite *) sprite)->SetHighlight(true);
            current_monster = ((MonsterSprite *) sprite);
            selection_mode = SELECTMODE_GENERAL;
            selection_side = PLAYER;
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
        if (((MonsterSprite *) sprite)->monster->health <= 0)
        {
            if (side == PLAYER)
            {}
            else
            {
                ((MonsterSprite *) sprite)->Kill();
                // release new monster if available
                // increase XP
            }
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
                    DrawRectangleRoundedPro(
                            text_rect, 0.2f, 10, COLORS["dark white"], true, true, false, false);
                }
                else if (CheckCollisionPointRec(
                                 Vector2Add(GetRectMidBottom(bg_rect), {0, -1}),
                                 text_rect.rectangle))
                {
                    DrawRectangleRoundedPro(
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
    for (auto *monster: monster_data[PLAYER])
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
                    DrawRectangleRoundedPro(
                            item_bg_rect, 0.4f, 10, COLORS["dark white"], true, true, false, false);
                }
                else if (CheckCollisionPointRec(
                                 Vector2Add(GetRectMidBottom(bg_rect), {0, -1}),
                                 item_bg_rect.rectangle))
                {
                    DrawRectangleRoundedPro(
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
