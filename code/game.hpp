#pragma once
#include <rygame.hpp>
#include "battle.hpp"
#include "dialogtree.hpp"
#include "entities.hpp"
#include "evolution.hpp"
#include "groups.hpp"
#include "monster.hpp"
#include "monsterindex.hpp"
#include "transitiontarget.hpp"


class Game
{
public:

    Game();
    ~Game();
    void run();

    Player player{};

private:

    void ImportAssets();
    void Setup(const std::string &map_name, const std::string &player_start_position);
    void UnloadResources();
    void Input();
    void CreateDialog(Character *character);
    void EndDialog(Character *character);
    void TransitionCheck();
    void TintScreen(float dt);
    void EndBattle(Character *character);
    // monster encounters
    void CheckMonster();
    void MonsterEncounter();
    void CheckEvolution();
    void EndEvolution();

    rg::Surface *display_surface{};
    std::unordered_map<std::string, rl::tmx_map *> tmx_maps{};
    std::unordered_map<unsigned int, rg::Surface> map_tiles_surfaces_;

    // overworld frames
    rg::Frames waterFrames{};
    std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>> cost_dict{};
    std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>> characters_dict{};
    std::vector<rg::Surface> surfaces_{};
    std::vector<Sprite> sprites_{};
    std::vector<AnimatedSprite> animated_sprites_{};
    std::vector<CollidableSprite> collidable_sprites_{};
    std::vector<TransitionSprite> transition_sprites_{};
    std::vector<BorderSprite> border_sprites_{};
    std::vector<MonsterPatchSprite> monster_patch_sprites_{};
    std::vector<Character> characters_{};

    // Groups
    AllSprites all_sprites{};
    rg::sprite::Group collision_sprites{};
    rg::sprite::Group character_sprites{};
    rg::sprite::Group transition_sprites{};
    rg::sprite::Group monster_sprites{};

    std::unordered_map<std::string, rg::font::Font> fonts{};
    DialogTree dialog_tree{};
    Battle battle{};

    TransitionTarget transition_target{};
    rg::Surface tint_surf{};
    TINT_MODE tint_mode = UNTINT;
    float tint_progress = 0.0f;
    // float tint_direction = -1;
    float tint_speed = 600.0f;

    std::unordered_map<int, Monster> player_monsters{};
    std::unordered_map<int, Monster> encounter_monsters{};
    // std::unordered_map<int, Monster>> dummy_monsters;
    MonsterIndex monster_index{};
    bool index_open{};
    std::unordered_map<std::string, rg::Surface> monster_icons{};
    // Name, Idle/Attack, Frames
    std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> monster_frames
            {};
    // Name, Idle/Attack, Frames
    std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> outline_frames
            {};
    std::unordered_map<std::string, rg::Surface> ui_icons{};
    std::unordered_map<AttackAnimation, rg::Frames> attack_frames{};

    std::unordered_map<std::string, rg::Surface> bg_frames{};

    rg::Timer encounter_timer{};

    Evolution evolution{};
    std::vector<rg::Surface> star_animation_surfs{};

    std::unordered_map<std::string, rg::mixer::Sound> audio{};
};
