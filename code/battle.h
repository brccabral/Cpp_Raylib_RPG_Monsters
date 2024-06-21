#pragma once
#include <vector>
#include "groups.h"


class Battle
{
public:

    Battle(const std::vector<Monster> &player_monsters,
           const std::vector<Monster> &opponent_monsters,
           const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                   &monsters_frames,
           const Texture2D &bg_surf, const std::map<std::string, Font> &fonts);
    ~Battle();
    void Update(double dt);
    void Setup();
    void CreateMonster(const Monster &monster, int index, int pos_index, const std::string &entity);

    // check what is the first monster to get Initiative = 100
    // it depends on monster speed
    // when the first monter achieves 100, pause the others
    void CheckActive();
    void CheckActiveGroup(const SpriteGroup *group);
    void UpdateAllMonsters(bool do_pause) const;

private:

    Texture2D bg_surf;
    std::map<std::string, std::map<std::string, std::vector<TiledTexture>>> monsters_frames;
    std::map<std::string, Font> fonts;
    std::map<std::string, std::vector<Monster>> monster_data;

    // gorups
    BattleSprites *battle_sprites = nullptr;
    SpriteGroup *player_sprites = nullptr;
    SpriteGroup *opponent_sprites = nullptr;

    // control
    MonsterSprite *current_monster = nullptr;
};
