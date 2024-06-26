#include "dialogtree.h"


DialogTree::DialogTree(
        Character *character_, Player *player_, const std::vector<SpriteGroup *> &grps,
        const Font &font_, const std::function<void(Character *)> &end_dialog)
    : character(character_), player(player_), groups(grps), font(font_), end_dialog(end_dialog)
{
    dialog = character->GetDialog();
    dialog_num = dialog.size();

    currentDialog = new DialogSprite(dialog[dialog_index], character, groups, font);
}

DialogTree::~DialogTree()
{
    if (currentDialog)
    {
        delete currentDialog;
        currentDialog = nullptr;
    }
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
            end_dialog(character);
        }
    }
    // PollInputEvents(); // if pool events here the game won't exit during dialog
}

void DialogTree::Update()
{
    dialog_timer.Update();
    Input();
}
