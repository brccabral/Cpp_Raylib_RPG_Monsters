#include "tiledfont.h"

#include "raylib_utils.h"
#include "settings.h"

TiledFont::TiledFont(
        const char *text, const Font &font, const Vector2 pos, const Vector2 padding,
        const float roundness, const float spacing)
    : text(text), font(font), padding(padding), roundness(roundness), spacing(spacing)
{
    const Vector2 text_size = MeasureTextEx(font, text, font.baseSize, spacing);
    rect = {pos, text_size};
    RectInflate(rect, padding.x * 2, padding.y * 2);
}

void TiledFont::Draw() const
{
    if (roundness)
    {
        DrawRectangleRounded(rect.rectangle, roundness, 10, COLORS["white"]);
    }
    else
    {
        DrawRectangleRec(rect.rectangle, COLORS["white"]);
    }
    DrawTextEx(font, text, Vector2Add(rect.pos, padding), font.baseSize, spacing, COLORS["black"]);
}
