#include <cstring>
#include <iostream>
#include "battle.h"


Battle::Battle(
        const std::vector<Monster> &player_monsters, const std::vector<Monster> &opponent_monsters,
        const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                &monsters_frames,
        const Texture2D &bg_surf, const std::map<std::string, Font> &fonts)
    : bg_surf(bg_surf), monsters_frames(monsters_frames), fonts(fonts),
      monster_data({{"player", player_monsters}, {"opponent", opponent_monsters}})
{
    battle_sprites = new BattleSprites();
    player_sprites = new SpriteGroup();
    opponent_sprites = new SpriteGroup();

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
    // Update Sprites before drawing into `display_surface`, as
    // some sprites open renderes to draw into
    battle_sprites->Update(dt);
    CheckActive();

    BeginTextureModeC(display_surface, BLACK);
    DrawTexture(bg_surf, 0, 0, WHITE);
    battle_sprites->Draw();
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
        const Monster &monster, const int index, const int pos_index, const std::string &entity)
{
    const auto frames = monsters_frames[monster.name];
    Vector2 pos;
    std::vector<SpriteGroup *> groups{};
    MonsterSprite *monster_sprite;
    MonsterNameSprite *name_sprite;
    Vector2 name_pos;
    Vector2 level_pos;
    if (std::strcmp(entity.c_str(), "player") == 0)
    {
        pos = BATTLE_POSITIONS["left"][pos_index];
        groups = {battle_sprites, player_sprites};
        monster_sprite = new MonsterSprite(pos, frames, groups, monster, index, pos_index, entity);
        monster_sprite->FlipH();
        name_pos = Vector2Add(GetRectMidLeft(monster_sprite->rect), {-40, -70});
        name_sprite =
                new MonsterNameSprite(name_pos, monster_sprite, {battle_sprites}, fonts["regular"]);
        level_pos = GetRectBottomLeft(name_sprite->rect);
    }
    else
    {
        pos = BATTLE_POSITIONS["right"][pos_index];
        groups = {battle_sprites, opponent_sprites};
        monster_sprite = new MonsterSprite(pos, frames, groups, monster, index, pos_index, entity);
        name_pos = Vector2Add(GetRectMidRight(monster_sprite->rect), {-60, -70});
        name_sprite =
                new MonsterNameSprite(name_pos, monster_sprite, {battle_sprites}, fonts["regular"]);
        level_pos = GetRectBottomRight(name_sprite->rect);
    }
    new MonsterLevelSprite(entity, level_pos, monster_sprite, {battle_sprites}, fonts["small"]);
    new MonsterStatsSprite(
            Vector2Add(GetRectMidBottom(monster_sprite->rect), {0.20}), monster_sprite, {150, 48},
            {battle_sprites}, fonts["small"]);
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
        if (((MonsterSprite *) sprite)->monster.initiative >= 100)
        {
            UpdateAllMonsters(true);
            ((MonsterSprite *) sprite)->monster.initiative = 0;
            ((MonsterSprite *) sprite)->SetHighlight(true);
            current_monster = ((MonsterSprite *) sprite);
        }
    }
}

void Battle::UpdateAllMonsters(const bool do_pause) const
{
    for (const auto *sprite: player_sprites->sprites)
    {
        ((MonsterSprite *) sprite)->monster.paused = do_pause;
    }
    for (const auto *sprite: opponent_sprites->sprites)
    {
        ((MonsterSprite *) sprite)->monster.paused = do_pause;
    }
}
