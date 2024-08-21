#pragma once
#include "groups.hpp"


#include <rygame.hpp>
#include "monster.hpp"
#include "sprite.hpp"


class Battle
{
public:

    Battle(std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
           std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
                   *monster_frames,
           std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
                   *outline_frames,
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

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    std::map<int, Monster> *player_monsters;
    std::map<int, Monster> *opponent_monsters;
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames;
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *outline_frames;
    std::shared_ptr<rg::Surface> bg_surf;
    std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts;

    BattleSprites battle_sprites{};
    rg::sprite::Group player_sprites{};
    rg::sprite::Group opponent_sprites{};

    std::shared_ptr<MonsterSprite> current_monster;
};
