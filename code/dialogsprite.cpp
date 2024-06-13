#include "dialogsprite.h"
#include <raymath.h>
#include "raylib_utils.h"
#include <utility>

static void DrawTextBoxed(
        const Font &font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap,
        Color tint); // Draw text using font inside rectangle limits
static void DrawTextBoxedSelectable(
        const Font &font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint,
        int selectStart, int selectLength, Color selectTint,
        Color selectBackTint); // Draw text using font inside rectangle limits with support for text selection

DialogSprite::DialogSprite(
        std::string msg, const Character *trainer, const std::vector<SpriteGroup *> &grps, const Font &font_)
    : SimpleSprite(grps), message(std::move(msg)), font(font_)
{
    z = WORLD_LAYERS["top"];
    type = DIALOGSPRITE;

    constexpr unsigned long padding = 10;
    rect.width = std::max(TextLength(message.c_str()) * 15 + padding * 2, 300ul);
    rect.height = FONT_SIZE + padding * 2;
    // dialog midbottom to trainer midtop
    const auto [x, y] = GetRectMidtop(trainer->rect);
    rect.x = x - rect.width / 2;
    rect.y = y - rect.height - 10;

    y_sort = GetRectCenter(rect).y;
}

void DialogSprite::Draw(const Vector2 offset) const
{
    const auto [x, y] = Vector2Add({rect.x, rect.y}, offset);
    Rectangle newRect = rect;
    newRect.x = x;
    newRect.y = y;

    // DrawTextEx(font, message.c_str(), pos, FONT_SIZE, 2, COLORS["black"]);
    DrawRectangleLinesEx(newRect, 2, COLORS["black"]);
    MoveRect(newRect, {5, 5});
    DrawTextBoxed(font, message.c_str(), newRect, FONT_SIZE, 2, true, BLACK);
    // DrawRectangleLinesEx(newRect, 2, COLORS["black"]);
    // DrawTextBoxed(font, message.c_str(), newRect, FONT_SIZE, 2, true, BLACK);
}


// Draw text using font inside rectangle limits
static void DrawTextBoxed(
        const Font &font, const char *text, const Rectangle rec, const float fontSize, const float spacing,
        const bool wordWrap, const Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(
        const Font &font, const char *text, const Rectangle rec, const float fontSize, const float spacing,
        const bool wordWrap, const Color tint, int selectStart, const int selectLength, const Color selectTint,
        const Color selectBackTint)
{
    const int length = TextLength(text); // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0.0f; // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f; // Offset X to next character to draw

    const float scaleFactor = fontSize / (float) font.baseSize; // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum
    {
        MEASURE_STATE = 0,
        DRAW_STATE = 1
    };
    int state = wordWrap ? MEASURE_STATE : DRAW_STATE;

    int startLine = -1; // Index where to begin drawing (where a line begins)
    int endLine = -1; // Index where to stop drawing (where a line ends)
    int lastk = -1; // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        const int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        const int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f)
            codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width * scaleFactor
                                                            : font.glyphs[index].advanceX * scaleFactor;

            if (i + 1 < length)
                glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec
        // container We store this info in startLine and endLine, then we change states, draw the text between those two
        // variables and change states again and again recursively until the end of the text (or until we get outside of
        // the container). When wordWrap is OFF we don't need the measure state so we go to the drawing state
        // immediately and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n'))
                endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1) ? i : endLine;
                if (i == endLine)
                    endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine)
                    endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n')
                state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                const int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += (font.baseSize + font.baseSize / 2.0f) * scaleFactor;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + font.baseSize / 2.0f) * scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize * scaleFactor) > rec.height)
                    break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec(
                            (Rectangle){
                                    rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth,
                                    (float) font.baseSize * scaleFactor},
                            selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(
                            font, codepoint, (Vector2){rec.x + textOffsetX, rec.y + textOffsetY}, fontSize,
                            isGlyphSelected ? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += (font.baseSize + font.baseSize / 2.0f) * scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                state = !state;
            }
        }

        textOffsetX += glyphWidth;
    }
}
