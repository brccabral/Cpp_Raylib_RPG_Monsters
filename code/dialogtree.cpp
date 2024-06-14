#include "dialogtree.h"

#include <iostream>


DialogTree::DialogTree(
        const Character *character_, Player *player_, const std::vector<SpriteGroup *> &grps, const Font &font_)
    : character(character_), player(player_), groups(grps), font(font_)
{
    dialog = character->GetDialog();
    dialog_num = dialog.size();

    currentDialog = new DialogSprite(dialog[dialog_index], character, groups, font);

    std::cout << dialog[0] << "\n";
}

void DialogTree::Input()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        if (currentDialog)
        {
            delete currentDialog;
            currentDialog = nullptr;
        }
        ++dialog_index;
        if (dialog_index < dialog_num)
        {
            SpriteGroup *g0 = groups[0];
            currentDialog = new DialogSprite(dialog[dialog_index], character, groups, font);
        }
        else
        {
            std::cout << "No more dialogs.\n";
        }
    }
}

void DialogTree::Update()
{
    Input();
}
