#pragma once
#include <vector>
#include "groups.h"


enum SelectionMode
{
    NONE = 0,
    GENERAL,
    MONSTER,
    ATTACKS,
    SWITCH,
    TARGET,
};

enum SelectionSide
{
    PLAYER = 0,
};

class Battle
{
public:

    Battle(const std::vector<Monster> &player_monsters,
           const std::vector<Monster> &opponent_monsters,
           const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                   &monsters_frames,
           const std::map<std::string, std::map<std::string, std::vector<TiledTexture>>>
                   &outline_frames,
           const std::map<std::string, Texture2D> &ui_frms, const Texture2D &bg_surf,
           const std::map<std::string, Font> &fonts);
    ~Battle();
    void Update(double dt);
    void Setup();
    void CreateMonster(const Monster &monster, int index, int pos_index, const std::string &entity);
    void Input();

    // check what is the first monster to get Initiative = 100
    // it depends on monster speed
    // when the first monter achieves 100, pause the others
    void CheckActive();
    void CheckActiveGroup(const SpriteGroup *group);
    void UpdateAllMonsters(bool do_pause) const;

    // ui
    void DrawUi();
    void DrawGeneral();

private:

    Texture2D bg_surf;
    std::map<std::string, std::map<std::string, std::vector<TiledTexture>>> monsters_frames;
    std::map<std::string, std::map<std::string, std::vector<TiledTexture>>> outline_frames;
    std::map<std::string, Texture2D> ui_frames;
    std::map<std::string, Font> fonts;
    std::map<std::string, std::vector<Monster>> monster_data;

    // gorups
    BattleSprites *battle_sprites = nullptr;
    SpriteGroup *player_sprites = nullptr;
    SpriteGroup *opponent_sprites = nullptr;

    // control
    MonsterSprite *current_monster = nullptr;
    SelectionMode selection_mode{};
    SelectionSide selection_side{};
    std::map<SelectionMode, int> indexes;
};
