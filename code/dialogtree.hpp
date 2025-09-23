#pragma once
#include "entities.hpp"
#include "groups.hpp"
#include "sprite.hpp"


class DialogTree
{
public:

    DialogTree() = default;
    DialogTree(
            Character *character,
            AllSprites *all_sprites, const rg::font::Font *font,
            const std::function<void(Character *char_)> &end_dialog);
    void Update();
    void Input();

    bool active{};

private:

    Character *character{};
    AllSprites *all_sprites{};
    const rg::font::Font *font{};
    std::vector<std::string> dialog{};
    int dialog_num{};
    int dialog_index{};

    DialogSprite current_dialog{};
    // we need timer because the first message is shown right away, and the KEY_SPACE
    // is still active in the same frame, moving the message to the next one
    // The Timer makes sure there is a wait time from Creation and KEY_SPACE
    rg::Timer dialog_timer = rg::Timer(0.5f, false, true);

    std::function<void(Character *character)> end_dialog{};
};
