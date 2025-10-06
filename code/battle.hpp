#pragma once
#include <rygame.hpp>
#include "groups.hpp"
#include "monster.hpp"
#include "sprite.hpp"


class Battle
{
public:

    Battle() = default;
    Battle(
            std::map<int, Monster> *player_monsters,
            std::map<int, Monster> *opponent_monsters,
            std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *
            monster_frames,
            std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *
            outline_frames,
            std::unordered_map<std::string, rg::Surface> *monster_icons,
            std::unordered_map<std::string, rg::Surface> *ui_icons,
            std::unordered_map<AttackAnimation, rg::Frames> *attack_frames,
            rg::Surface *bg_surf,
            std::unordered_map<std::string, rg::font::Font> *fonts,
            const std::function<void(Character *c)> &endBattle,
            Character *character,
            std::unordered_map<std::string, rg::mixer::Sound> *sounds);
    Battle(const Battle &other) = delete;
    Battle &operator=(const Battle &other) = delete;
    Battle(Battle &&other) noexcept;
    Battle &operator=(Battle &&other) noexcept;
    void Update(float dt);

    bool battle_over = true;

private:

    void Setup();
    void CreateMonster(
            Monster *monster, int index, int pos_index,
            SelectionSide entity);
    // Battle System
    // check what is the first monster to get Initiative = 100
    // it depends on monster speed
    // when the first monster achieves 100, pause the others
    void CheckActive();
    void CheckActiveGroup(rg::sprite::Group *group, SelectionSide side);
    void UpdateAllMonsters(bool do_pause);
    void Input();
    void
    ApplyAttack(const MonsterSprite *target_sprite, Attack attack, float amount);
    void CheckDeath();
    void CheckDeathGroup(rg::sprite::Group *group, SelectionSide side);
    void CheckEndBattle();

    // ui
    void DrawUi();
    void DrawGeneral();
    void DrawAttacks();
    void DrawSwitch();

    void UpdateTimers();
    void OpponentAttack();
    void SetAttack(rg::sprite::Group *group, Attack ability) const;

    rg::Surface *display_surface = &rg::display::GetSurface();
    // all player's monsters
    std::map<int, Monster> *player_monsters{};
    // all opponents's monsters
    std::map<int, Monster> *opponent_monsters{};
    std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *monster_frames
            {};
    std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> *outline_frames
            {};
    std::unordered_map<std::string, rg::Surface> *monster_icons{};
    std::unordered_map<std::string, rg::Surface> *ui_icons{};
    rg::Surface *bg_surf{};
    std::unordered_map<std::string, rg::font::Font> *fonts{};

    BattleSprites battle_sprites{};
    // player monsters in battle
    rg::sprite::Group player_sprites{};
    // opponents monsters in battle
    rg::sprite::Group opponent_sprites{};

    struct BattleMonster
    {
        MonsterSprite monster_sprite;
        MonsterOutlineSprite monster_outline_sprite;
        MonsterNameSprite monster_name_sprite;
        MonsterLevelSprite monster_level_sprite;
        MonsterStatsSprite monster_stats_sprite;
    };

    std::vector<BattleMonster> battle_monster{};
    std::vector<TimedSprite> timed_sprites_{};
    std::vector<AttackSprite> attack_sprites_{};

    MonsterSprite *current_monster{};

    SelectionMode selection_mode{};
    SelectionSide selection_side{};
    std::unordered_map<SelectionMode, int> indexes{};

    // player extra monsters
    std::map<int, Monster *> available_monsters{};
    // opponent extra monsters
    std::map<int, Monster *> opponent_extra_monsters{};
    Attack selected_attack = ATTACK_NONE;
    std::unordered_map<AttackAnimation, rg::Frames> *attack_frames{};
    std::unordered_map<std::string, rg::Timer> timers{};
    bool monsters_paused{};

    std::function<void(Character *character)> endBattle{};
    Character *character{};

    std::unordered_map<std::string, rg::mixer::Sound> *sounds{};
    std::vector<rg::Surface> abilities_surfaces{};
    std::vector<rg::Surface> switch_monster_surfaces{};
};
