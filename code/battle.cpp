#include "battle.hpp"
#include "sprite.hpp"


Battle::Battle(
        std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *outline_frames,
        const std::shared_ptr<rg::Surface> &bg_surf,
        std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts)
    : player_monsters(player_monsters), opponent_monsters(opponent_monsters),
      monster_frames(monster_frames), outline_frames(outline_frames), bg_surf(bg_surf), fonts(fonts)
{
    Setup();
}

void Battle::Update(const float dt)
{
    // updates
    battle_sprites.Update(dt);
    CheckActive();

    // drawing
    display_surface->Blit(bg_surf, rg::math::Vector2{0, 0});
    battle_sprites.Draw(current_monster);
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

void Battle::CheckActiveGroup(const rg::sprite::Group *group, SelectionSide side)
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
