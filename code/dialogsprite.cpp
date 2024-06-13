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

    DrawRectangle(newRect.x, newRect.y, newRect.width, newRect.height, COLORS["pure white"]);
    MoveRect(newRect, {5, 5});
    DrawTextEx(font, message.c_str(), {newRect.x, newRect.y}, FONT_SIZE, 2, COLORS["black"]);
}
