#pragma once
#include "entities.h"
#include "groups.h"

class DialogTree
{
public:

    DialogTree(Character *character_, Player *player_, AllSprites *all_sprites_, const Font &font_);

private:

    Character *character;
    Player *player;
    AllSprites *all_sprites;
    Font font;
};
