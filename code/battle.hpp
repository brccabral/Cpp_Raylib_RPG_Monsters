#pragma once
#include "groups.hpp"


#include <rygame.hpp>
#include "monster.hpp"
#include "sprite.hpp"


class Battle
{
public:

    Battle(std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
           std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
                   *monster_frames,
           std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
                   *outline_frames,
           std::map<std::string, std::shared_ptr<rg::Surface>> *monster_icons,
           std::map<std::string, std::shared_ptr<rg::Surface>> *ui_icons,
           std::map<AttackAnimation, std::shared_ptr<rg::Frames>> *attack_frames,
           const std::shared_ptr<rg::Surface> &bg_surf,
           std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts);
    void Update(float dt);

private:

    void Setup();
    void CreateMonster(Monster *monster, int index, int pos_index, SelectionSide entity);
    // Battle System
    // check what is the first monster to get Initiative = 100
    // it depends on monster speed
    // when the first monster achieves 100, pause the others
    void CheckActive();
    void CheckActiveGroup(const rg::sprite::Group *group, SelectionSide side);
    void UpdateAllMonsters(bool do_pause) const;
    void Input();
    void
    ApplyAttack(const std::shared_ptr<MonsterSprite> &target_sprite, Attack attack, float amount);
    void CheckDeath();
    void CheckDeathGroup(const rg::sprite::Group *group, SelectionSide side);

    // ui
    void DrawUi();
    void DrawGeneral();
    void DrawAttacks();
    void DrawSwitch();

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    std::map<int, Monster> *player_monsters;
    std::map<int, Monster> *opponent_monsters;
    std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>> *monster_frames;
    std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>> *outline_frames;
    std::map<std::string, std::shared_ptr<rg::Surface>> *monster_icons;
    std::map<std::string, std::shared_ptr<rg::Surface>> *ui_icons;
    std::shared_ptr<rg::Surface> bg_surf;
    std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts;

    BattleSprites battle_sprites{};
    rg::sprite::Group player_sprites{};
    rg::sprite::Group opponent_sprites{};

    std::shared_ptr<MonsterSprite> current_monster;

    SelectionMode selection_mode{};
    SelectionSide selection_side{};
    std::map<SelectionMode, int> indexes;

    std::map<int, Monster> available_monsters;
    Attack selected_attack = ATTACK_NONE;
    std::map<AttackAnimation, std::shared_ptr<rg::Frames>> *attack_frames;
};
