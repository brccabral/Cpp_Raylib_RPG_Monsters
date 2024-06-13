#include "dialogtree.h"

#include <iostream>


DialogTree::DialogTree(Character *character_, Player *player_, AllSprites *all_sprites_, const Font &font_)
    : character(character_), player(player_), all_sprites(all_sprites_), font(font_)
{
    const std::vector<std::string> dialog = character->GetDialog();
    std::cout << dialog[0] << "\n";
}
