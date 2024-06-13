#include "dialogsprite.h"
#include <raymath.h>
#include "raylib_utils.h"
#include <utility>


DialogSprite::DialogSprite(
        std::string msg, const Character *trainer, const std::vector<SpriteGroup *> &grps, const Font &font_)
    : SimpleSprite(grps), message(std::move(msg)), font(font_)
{
    z = WORLD_LAYERS["top"];
    type = DIALOGSPRITE;

    constexpr float padding = 10.0f;
    // rect.width = std::max(TextLength(message.c_str()) * 16 + padding * 2, 300ul);
    textsize = MeasureTextEx(font, message.c_str(), FONT_SIZE, 2);
    rect.width = std::max(textsize.x + padding * 2, 300.0f);
    rect.height = textsize.y + padding * 2;
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

    DrawRectangleRounded(newRect, 0.3, 10, COLORS["pure white"]);

    // centralize text inside box
    const auto dif = Vector2Subtract({newRect.width, newRect.height}, textsize);
    MoveRect(newRect, Vector2Scale(dif, 0.5f));
    DrawTextEx(font, message.c_str(), {newRect.x, newRect.y}, FONT_SIZE, 2, COLORS["black"]);
}
