#pragma once
#include "raylib_utils.h"


#include <raylib.h>


class TiledFont
{
public:

    TiledFont(
            const char *text, const Font &font, Vector2 pos, Vector2 padding = {},
            float roundness = 0.0f, float spacing = 1.0f);
    void Draw() const;

    RectangleU rect{};

private:

    const char *text;
    Font font;
    Vector2 padding;
    float roundness;
    float spacing;
};
