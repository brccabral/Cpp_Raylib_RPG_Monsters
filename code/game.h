#pragma once
#include "DialogTree.h"


#include <rygame.h>
#include "entities.h"
#include "groups.h"


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

    std::shared_ptr<rg::Surface> display_surface = nullptr;
    std::map<std::string, rl::tmx_map *> tmx_maps;

    // overworld frames
    std::shared_ptr<rg::Frames> waterFrames = nullptr;
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> cost_dict;
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> characters_dict;

    // Groups
    std::shared_ptr<AllSprites> all_sprites;
    std::shared_ptr<rg::sprite::Group> collision_sprites;
    std::shared_ptr<rg::sprite::Group> character_sprites;
    std::shared_ptr<rg::sprite::Group> transition_sprites;

    std::map<std::string, std::shared_ptr<rg::font::Font>> fonts;
    std::shared_ptr<DialogTree> dialog_tree = nullptr;

    std::pair<std::string, std::string> transition_target;
    std::shared_ptr<rg::Surface> tint_surf;
    std::string tint_mode{"untint"};
    // float tint_progress = 255.0f;
    float tint_progress = 0.0f;
    // float tint_direction = -1;
    float tint_speed = 600.0f;
};
