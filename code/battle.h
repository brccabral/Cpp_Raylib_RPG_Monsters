#pragma once
#include <vector>
#include "monster.h"
#include "sprite.h"
#include "support.h"


class BattleSprites; // forward declaration

class Battle
{
public:

    Battle(const std::vector<Monster *> &player_monsters,
           const std::vector<Monster *> &opponent_monsters,
           const std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>>
                   &monsters_frames,
           const std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>>
                   &outline_frames,
           const std::map<std::string, Texture2D> &ui_frms, const Texture2D &bg_surf,
           const std::map<std::string, Texture2D> &monster_icons,
           const std::map<AttackAnimation, std::vector<TiledTexture>> &attack_animation_frms,
           const std::map<std::string, Font> &fonts);
    ~Battle();
    void Update(double dt);
    void Setup();
    void CreateMonster(Monster *monster, int index, int pos_index, SelectionSide entity);
    void Input();

    // Battle System
    // check what is the first monster to get Initiative = 100
    // it depends on monster speed
    // when the first monter achieves 100, pause the others
    void CheckActive();
    void CheckActiveGroup(const SpriteGroup *group);
    void UpdateAllMonsters(bool do_pause) const;
    void ApplyAttack(MonsterSprite *target_sprite, Attack attack, float amount);

    // ui
    void DrawUi();
    void DrawGeneral();
    void DrawAttacks();
    void DrawSwitch();

private:

    Texture2D bg_surf;
    std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>> monsters_frames;
    std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>> outline_frames;
    std::map<std::string, Texture2D> ui_frames;
    std::map<std::string, Font> fonts;
    std::map<SelectionSide, std::vector<Monster *>> monster_data;
    std::vector<Monster *> available_monsters;
    std::map<std::string, Texture2D> monster_icons;
    std::map<AttackAnimation, std::vector<TiledTexture>> attack_animation_frames;

    // groups
    BattleSprites *battle_sprites = nullptr;
    SpriteGroup *player_sprites = nullptr;
    SpriteGroup *opponent_sprites = nullptr;

    // control
    MonsterSprite *current_monster = nullptr;
    SelectionMode selection_mode{};
    // some attacks like "defense"/"healing" are targeting the player
    SelectionSide selection_side{};
    Attack selected_attack = ATTACK_NONE;
    std::map<SelectionMode, int> indexes;
};
