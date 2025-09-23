#include "dialogtree.hpp"


DialogTree::DialogTree(
        Character *character, const Player *player,
        AllSprites *all_sprites, const rg::font::Font *font,
        const std::function<void(Character *char_)> &end_dialog)
    : active(true), character(character), player(player), all_sprites(all_sprites), font(font),
      end_dialog(end_dialog)
{
    dialog = character->GetDialog();
    dialog_num = dialog.size();

    current_dialog = DialogSprite(dialog[dialog_index], character, font);
    current_dialog.add(all_sprites);
}

void DialogTree::Update()
{
    dialog_timer.Update();
    Input();
}

void DialogTree::Input()
{
    if (IsKeyPressed(rl::KEY_SPACE) && !dialog_timer.active)
    {
        current_dialog.Kill();
        dialog_index += 1;
        if (dialog_index < dialog_num)
        {
            current_dialog =
                    DialogSprite(dialog[dialog_index], character, font);
            current_dialog.add(all_sprites);
            dialog_timer.Activate();
        }
        else
        {
            end_dialog(character);
        }
    }
}
