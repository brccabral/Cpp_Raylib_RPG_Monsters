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

    rect.width = message.size() * FONT_SIZE;
    rect.height = FONT_SIZE;
    // dialog midbottom to trainer midtop
    rect.x = trainer->rect.x - rect.width / 2;
    rect.y = trainer->rect.y - rect.height - 10;

    y_sort = GetRectCenter(rect).y;
}

void DialogSprite::Draw(const Vector2 offset) const
{
    const auto pos = Vector2Add({rect.x, rect.y}, offset);

    DrawTextEx(font, message.c_str(), pos, FONT_SIZE, 20, COLORS["black"]);
}
