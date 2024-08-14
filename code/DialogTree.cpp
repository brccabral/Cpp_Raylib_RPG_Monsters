#include "DialogTree.h"
#include "sprite.h"


DialogTree::DialogTree(
        const std::shared_ptr<Character> &character, const std::shared_ptr<Player> &player,
        const std::shared_ptr<AllSprites> &all_sprites, const std::shared_ptr<rg::font::Font> &font)
    : character(character), player(player), all_sprites(all_sprites), font(font)
{
    dialog = character->GetDialog();
    dialog_num = dialog.size();

    current_dialog = std::make_shared<DialogSprite>(dialog[dialog_index], character, this->font);
    current_dialog->add(all_sprites.get());
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
        current_dialog->Kill();
        dialog_index += 1;
        if (dialog_index < dialog_num)
        {
            current_dialog =
                    std::make_shared<DialogSprite>(dialog[dialog_index], character, this->font);
            current_dialog->add(all_sprites.get());
            dialog_timer.Activate();
        }
        else
        {}
    }
}
