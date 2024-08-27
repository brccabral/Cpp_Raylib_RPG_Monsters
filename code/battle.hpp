#pragma once
#include <rygame.hpp>
#include "groups.hpp"
#include "monster.hpp"
#include "sprite.hpp"


class Battle
{
public:

    Battle(std::map<int, std::shared_ptr<Monster>> *player_monsters,
           std::map<int, std::shared_ptr<Monster>> *opponent_monsters,
           std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>>
                   *monster_frames,
           std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>>
                   *outline_frames,
           std::map<std::string, rg::Surface_Ptr> *monster_icons,
           std::map<std::string, rg::Surface_Ptr> *ui_icons,
           std::map<AttackAnimation, rg::Frames_Ptr> *attack_frames,
           const rg::Surface_Ptr &bg_surf,
           std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts,
           const std::function<void(const std::shared_ptr<Character> &c)> &endBattle,
           const std::shared_ptr<Character> &character,
           std::map<std::string, std::shared_ptr<rg::mixer::Sound>> *sounds);
    void Update(float dt);

private:

    void Setup();
    void CreateMonster(
            const std::shared_ptr<Monster> &monster, int index, int pos_index,
            SelectionSide entity);
    // Battle System
    // check what is the first monster to get Initiative = 100
    // it depends on monster speed
    // when the first monster achieves 100, pause the others
    void CheckActive();
    void CheckActiveGroup(const rg::sprite::Group *group, SelectionSide side);
    void UpdateAllMonsters(bool do_pause);
    void Input();
    void
    ApplyAttack(const std::shared_ptr<MonsterSprite> &target_sprite, Attack attack, float amount);
    void CheckDeath();
    void CheckDeathGroup(const rg::sprite::Group *group, SelectionSide side);
    void CheckEndBattle();

    // ui
    void DrawUi();
    void DrawGeneral();
    void DrawAttacks();
    void DrawSwitch();

    void UpdateTimers();
    void OpponentAttack() const;

    rg::Surface_Ptr display_surface = rg::display::GetSurface();
    std::map<int, std::shared_ptr<Monster>> *player_monsters;
    std::map<int, std::shared_ptr<Monster>> *opponent_monsters;
    std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> *monster_frames;
    std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> *outline_frames;
    std::map<std::string, rg::Surface_Ptr> *monster_icons;
    std::map<std::string, rg::Surface_Ptr> *ui_icons;
    rg::Surface_Ptr bg_surf;
    std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts;

    BattleSprites battle_sprites{};
    rg::sprite::Group player_sprites{};
    rg::sprite::Group opponent_sprites{};

    std::shared_ptr<MonsterSprite> current_monster;

    SelectionMode selection_mode{};
    SelectionSide selection_side{};
    std::map<SelectionMode, int> indexes;

    std::map<int, std::shared_ptr<Monster>> available_monsters;
    Attack selected_attack = ATTACK_NONE;
    std::map<AttackAnimation, rg::Frames_Ptr> *attack_frames;

    std::map<std::string, rg::Timer> timers;
    bool monsters_paused{};

    bool battle_over{};
    const std::function<void(const std::shared_ptr<Character> &character)> endBattle;
    const std::shared_ptr<Character> character;

    std::map<std::string, std::shared_ptr<rg::mixer::Sound>> *sounds;
};
