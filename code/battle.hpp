#pragma once
#include <rygame.hpp>
#include "monster.hpp"


class Battle
{
public:

    Battle(std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
           std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
                   *monster_frames,
           const std::shared_ptr<rg::Surface> &bg_surf,
           std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts);
    void Update(float dt);

private:

    void Setup();
    void CreateMonster(Monster *monster, int index, int pos_index, const std::string &entity);

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    std::map<int, Monster> *player_monsters;
    std::map<int, Monster> *opponent_monsters;
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames;
    std::shared_ptr<rg::Surface> bg_surf;
    std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts;

    rg::sprite::Group battle_sprites{};
    rg::sprite::Group player_sprites{};
    rg::sprite::Group opponent_sprites{};
};
