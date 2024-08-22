#include "battle.hpp"
#include "sprite.hpp"


Battle::Battle(
        std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *outline_frames,
        std::map<std::string, std::shared_ptr<rg::Surface>> *ui_icons,
        const std::shared_ptr<rg::Surface> &bg_surf,
        std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts)
    : player_monsters(player_monsters), opponent_monsters(opponent_monsters),
      monster_frames(monster_frames), outline_frames(outline_frames), ui_icons(ui_icons),
      bg_surf(bg_surf), fonts(fonts)
{
    indexes[SELECTMODE_GENERAL] = 0;
    indexes[SELECTMODE_MONSTER] = 0;
    indexes[SELECTMODE_ATTACKS] = 0;
    indexes[SELECTMODE_SWITCH] = 0;
    indexes[SELECTMODE_TARGET] = 0;

    Setup();
}

void Battle::Update(const float dt)
{
    Input();
    // updates
    battle_sprites.Update(dt);
    CheckActive();

    // drawing
    display_surface->Blit(bg_surf, rg::math::Vector2{0, 0});
    battle_sprites.Draw(current_monster);
    DrawUi();
}

void Battle::Setup()
{
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
        }
    }
}

void Battle::CreateMonster(Monster *monster, int index, int pos_index, SelectionSide entity)
{
    auto frames = (*monster_frames)[monster->name];
    auto outlines = (*outline_frames)[monster->name];
    rg::math::Vector2 pos;
    auto groups = std::vector<rg::sprite::Group *>{};

    if (entity == PLAYER)
    {
        pos = BATTLE_POSITIONS["left"][pos_index];
        groups = {&battle_sprites, &player_sprites};
        for (auto &[state, frame]: frames)
        {
            frames[state] = rg::transform::Flip(frame, true, false);
        }
        for (auto &[state, frame]: outlines)
        {
            outlines[state] = rg::transform::Flip(frame, true, false);
        }
    }
    else
    {
        pos = BATTLE_POSITIONS["right"][pos_index];
        groups = {&battle_sprites, &opponent_sprites};
    }
    const auto monster_sprite =
            std::make_shared<MonsterSprite>(pos, frames, monster, index, pos_index, entity);
    monster_sprite->add(groups);

    std::make_shared<MonsterOutlineSprite>(monster_sprite, outlines)->add(&battle_sprites);

    // ui
    rg::math::Vector2 name_pos;

    if (entity == PLAYER)
    {
        name_pos = monster_sprite->rect.midleft() + rg::math::Vector2{16, -70};
    }
    else
    {
        name_pos = monster_sprite->rect.midright() + rg::math::Vector2{-40, -70};
    }
    const auto name_sprite =
            std::make_shared<MonsterNameSprite>(name_pos, monster_sprite, (*fonts)["regular"]);
    name_sprite->add(&battle_sprites);

    rg::math::Vector2 anchor;
    if (entity == PLAYER)
    {
        anchor = name_sprite->rect.bottomleft();
    }
    else
    {
        anchor = name_sprite->rect.bottomright();
    }
    std::make_shared<MonsterLevelSprite>(entity, anchor, monster_sprite, (*fonts)["small"])
            ->add(&battle_sprites);
    std::make_shared<MonsterStatsSprite>(
            monster_sprite->rect.midbottom() + rg::math::Vector2{0, 20}, monster_sprite,
            rg::math::Vector2{150, 48}, (*fonts)["small"])
            ->add(&battle_sprites);
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
    for (const auto &sprite: group->Sprites())
    {
        const auto monster_sprite = std::dynamic_pointer_cast<MonsterSprite>(sprite);
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
        }
    }
}

void Battle::UpdateAllMonsters(const bool do_pause) const
{
    for (const auto &sprite: player_sprites.Sprites())
    {
        const auto monster_sprite = std::dynamic_pointer_cast<MonsterSprite>(sprite);
        monster_sprite->monster->paused = do_pause;
    }
    for (const auto &sprite: opponent_sprites.Sprites())
    {
        const auto monster_sprite = std::dynamic_pointer_cast<MonsterSprite>(sprite);
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
                break;
            }
            case SELECTMODE_SWITCH:
            {
                break;
            }
            case SELECTMODE_TARGET:
            {
                break;
            }
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
            if (selection_mode == SELECTMODE_GENERAL)
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
        std::shared_ptr<rg::Surface> surf;
        if (index == indexes[SELECTMODE_GENERAL])
        {
            surf = (*ui_icons)[battle_choice.icon + "_highlight"];
        }
        else
        {
            surf = rg::transform::GrayScale((*ui_icons)[battle_choice.icon]);
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
        v_offset = -(indexes[SELECTMODE_ATTACKS] - visible_attacks + 1) * item_height;
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
                (*fonts)["regular"]->render(ATTACK_DATA[abilities[index]].name.c_str(), text_color);
        auto text_rect = text_surf->GetRect().center(
                bg_rect.midtop() +
                rg::math::Vector2{0, item_height / 2 + index * item_height + v_offset});

        display_surface->Blit(text_surf, text_rect);
    }
}

void Battle::DrawSwitch()
{}
