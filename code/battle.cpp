#include "battle.hpp"

#include "sprite.hpp"


Battle::Battle(
        std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames,
        const std::shared_ptr<rg::Surface> &bg_surf,
        std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts)
    : player_monsters(player_monsters), opponent_monsters(opponent_monsters),
      monster_frames(monster_frames), bg_surf(bg_surf), fonts(fonts)
{
    Setup();
}

void Battle::Update(const float dt)
{
    display_surface->Blit(bg_surf, {0, 0});
    battle_sprites.Update(dt);
    battle_sprites.Draw();
}

void Battle::Setup()
{
    for (auto &[index, monster]: *player_monsters)
    {
        if (index <= 2)
        {
            CreateMonster(&monster, index, index, "player");
        }
    }
    for (auto &[index, monster]: *opponent_monsters)
    {
        if (index <= 2)
        {
            CreateMonster(&monster, index, index, "opponent");
        }
    }
}

void Battle::CreateMonster(Monster *monster, int index, int pos_index, const std::string &entity)
{
    auto frames = (*monster_frames)[monster->name];
    rg::math::Vector2 pos;
    auto groups = std::vector<rg::sprite::Group *>{};

    if (!std::strcmp(entity.c_str(), "player"))
    {
        pos = BATTLE_POSITIONS["left"][pos_index];
        groups = {&battle_sprites, &player_sprites};
        for (auto &[state, frame]: frames)
        {
            frames[state] = rg::transform::Flip(frame, true, false);
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

    // ui
    rg::math::Vector2 name_pos;

    if (!std::strcmp(entity.c_str(), "player"))
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
    if (!std::strcmp(entity.c_str(), "player"))
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
