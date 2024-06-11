#include "groups.h"

void AllSprites::Draw() const
{
    for (const auto &sprite: sprites)
    {
        sprite->Draw(offset);
    }
}
