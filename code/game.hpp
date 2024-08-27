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

    std::shared_ptr<Player> player = nullptr;

private:

    void ImportAssets();
    void Setup(const std::string &map_name, const std::string &player_start_position);
    void UnloadResources();
    void Input();
    void CreateDialog(const std::shared_ptr<Character> &character);
    void EndDialog(const std::shared_ptr<Character> &character);
    void TransitionCheck();
    void TintScreen(double dt);
    void EndBattle(const std::shared_ptr<Character> &character);
    // monster encounters
    void CheckMonster();
    void MonsterEncounter();
    void CheckEvolution();
    void EndEvolution();

    std::shared_ptr<rg::Surface> display_surface = nullptr;
    std::map<std::string, rl::tmx_map *> tmx_maps;

    // overworld frames
    std::shared_ptr<rg::Frames> waterFrames = nullptr;
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> cost_dict;
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> characters_dict;

    // Groups
    // need shared_ptr due to AllSprites loads images in constructor
    std::shared_ptr<AllSprites> all_sprites = nullptr;
    rg::sprite::Group collision_sprites{};
    rg::sprite::Group character_sprites{};
    rg::sprite::Group transition_sprites{};
    rg::sprite::Group monster_sprites{};

    std::map<std::string, std::shared_ptr<rg::font::Font>> fonts;
    std::shared_ptr<DialogTree> dialog_tree = nullptr;
    std::shared_ptr<Battle> battle = nullptr;

    std::shared_ptr<TransitionTarget> transition_target = nullptr;
    std::shared_ptr<rg::Surface> tint_surf;
    TINT_MODE tint_mode = UNTINT;
    float tint_progress = 0.0f;
    // float tint_direction = -1;
    float tint_speed = 600.0f;

    std::map<int, std::shared_ptr<Monster>> player_monsters;
    std::map<int, std::shared_ptr<Monster>> encounter_monsters;
    // std::map<int, std::shared_ptr<Monster>> dummy_monsters;
    std::shared_ptr<MonsterIndex> monster_index;
    bool index_open{};
    std::map<std::string, std::shared_ptr<rg::Surface>> monster_icons;
    std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
            monster_frames; // Name, Idle/Attack, Frames
    std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
            outline_frames; // Name, Idle/Attack, Frames
    std::map<std::string, std::shared_ptr<rg::Surface>> ui_icons;
    std::map<AttackAnimation, std::shared_ptr<rg::Frames>> attack_frames;

    std::map<std::string, std::shared_ptr<rg::Surface>> bg_frames;

    rg::Timer encounter_timer;

    std::shared_ptr<Evolution> evolution = nullptr;
    std::vector<std::shared_ptr<rg::Surface>> star_animation_surfs{};

    std::map<std::string, std::shared_ptr<rg::mixer::Sound>> audio;
};
