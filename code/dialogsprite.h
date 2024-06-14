#pragma once
#include <string>
#include "entities.h"


class DialogSprite : public SimpleSprite
{
public:

    DialogSprite(std::string msg, const Character *trainer, const std::vector<SpriteGroup *> &grps, const Font &font_);
    Texture2D *CreateImage() const;
    int z = WORLD_LAYERS["top"];
    std::string message;
    int y_sort{};
    const Font font;
    Vector2 textsize{};
};
