#pragma once
#include <raylib.h>
#include "raylib_utils.h"


class TiledFont
{
public:

    TiledFont() = default;

    TiledFont(
            const char *text, const Font &font, float spacing, Color fg_color, Vector2 pos = {},
            Color bg_color = BLANK, Vector2 padding = {}, float roundness = 0);

    void Draw() const;

    RectangleU rect{};

private:

    const char *text;
    Font font;
    float spacing;
    Color fg_color;
    Color bg_color;
    Vector2 padding;
    float roundness;
};
