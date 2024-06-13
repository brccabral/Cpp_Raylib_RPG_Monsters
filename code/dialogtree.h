#pragma once
#include "dialogsprite.h"
#include "entities.h"
#include "groups.h"

class DialogTree
{
public:

    DialogTree(const Character *character_, Player *player_, const std::vector<SpriteGroup *> &grps, const Font &font_);
    ~DialogTree();

private:

    const Character *character;
    Player *player;
    const Font font;
    std::vector<std::string> dialog;
    int dialog_num{};
    int dialog_index{};
    DialogSprite *currentDialog = nullptr;
};
