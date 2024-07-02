#pragma once
#include "dialogsprite.h"

class DialogTree
{
public:

    DialogTree(
            Character *character_, Player *player_, const std::vector<SpriteGroup *> &grps,
            const Font &font_, const std::function<void(Character *)> &end_dialog);
    ~DialogTree();
    void Input();
    void Update();

private:

    Character *character;
    Player *player;
    std::vector<SpriteGroup *> groups;
    const Font font;
    std::vector<std::string> dialog;
    int dialog_num{};
    int dialog_index{};
    DialogSprite *currentDialog = nullptr;
    Timer dialog_timer{0.5f, false, true};
    std::function<void(Character *)> end_dialog;
};
