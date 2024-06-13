#pragma once
#include "entities.h"
#include "groups.h"

struct Dialog
{
    std::vector<std::string> default_;
    std::vector<std::string> defeated;
};

struct Monster
{
    std::string name;
    int level;
};

struct CharacterData
{
    std::vector<Monster> monsters;
    Dialog dialog;
    std::vector<FacingDirection> directions;
    bool look_around;
    bool defeated;
    std::string biome;
};

static std::map<std::string, CharacterData> TRAINER_DATA = {};

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
