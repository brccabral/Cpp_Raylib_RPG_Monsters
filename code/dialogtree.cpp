#include "dialogtree.h"

#include <iostream>


DialogTree::DialogTree(
        const Character *character_, Player *player_, const std::vector<SpriteGroup *> &grps, const Font &font_)
    : character(character_), player(player_), font(font_)
{
    dialog = character->GetDialog();
    dialog_num = dialog.size();

    currentDialog = new DialogSprite(dialog[dialog_index], character, grps, font);

    std::cout << dialog[0] << "\n";
}

DialogTree::~DialogTree()
{
    delete currentDialog;
}
