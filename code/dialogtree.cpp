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
    if (IsKeyPressed(KEY_SPACE) && !dialog_timer.active)
    {
        if (currentDialog)
        {
            delete currentDialog;
            currentDialog = nullptr;
        }
        ++dialog_index;
        if (dialog_index < dialog_num)
        {
            currentDialog = new DialogSprite(dialog[dialog_index], character, groups, font);
            dialog_timer.Activate();
        }
        else
        {
            std::cout << "No more dialogs.\n";
        }
    }
}

void DialogTree::Update()
{
    dialog_timer.Update();
    Input();
}
