#pragma once
#include "entities.h"
#include "groups.h"
#include "sprite.h"

class DialogTree
{
public:

    DialogTree(
            const std::shared_ptr<Character> &character, const std::shared_ptr<Player> &player,
            const std::shared_ptr<AllSprites> &all_sprites,
            const std::shared_ptr<rg::font::Font> &font);

private:

    std::shared_ptr<Character> character;
    std::shared_ptr<Player> player;
    std::shared_ptr<AllSprites> all_sprites;
    std::shared_ptr<rg::font::Font> font;
    std::vector<std::string> dialog{};
    int dialog_num{};
    int dialog_index{};

    std::shared_ptr<DialogSprite> current_dialog;
};
