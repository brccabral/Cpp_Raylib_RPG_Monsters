#include "tiledfont.h"

TiledFont::TiledFont(
        const char *text, const Font &font, const float spacing, const Color fg_color,
        const Vector2 pos, const Color bg_color, const Vector2 padding, float roundness)
    : text(text), font(font), spacing(spacing), fg_color(fg_color), bg_color(bg_color),
      padding(padding), roundness(roundness)
{
    const Vector2 text_size = MeasureTextEx(font, text, font.baseSize, spacing);
    rect = {pos, text_size};
    RectInflate(rect, padding.x * 2, padding.y * 2);
}

void TiledFont::Draw() const
{
    if (!roundness)
    {
        DrawRectangleRec(rect.rectangle, bg_color);
    }
    else
    {
        DrawRectangleRounded(rect.rectangle, roundness, 10, bg_color);
    }
    DrawTextEx(font, text, Vector2Add(rect.pos, padding), font.baseSize, spacing, fg_color);
}
