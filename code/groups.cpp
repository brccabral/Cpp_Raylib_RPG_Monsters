#include "groups.h"
#include <raymath.h>
#include "settings.h"

void AllSprites::Draw(const Vector2 player_center)
{
    offset = Vector2Subtract(player_center, Vector2Scale({WINDOW_WIDTH, WINDOW_HEIGHT}, 0.5));
    offset = Vector2Scale(offset, -1);

    for (const auto &sprite: sprites)
    {
        sprite->Draw(offset);
    }
}
