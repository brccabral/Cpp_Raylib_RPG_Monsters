#pragma once
#include <raylib.h>

inline Vector2 GetRectCenter(const Rectangle rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

inline void RectToCenter(Rectangle &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width / 2;
    rect.y = pos.y - rect.height / 2;
}

inline void RectToTopleft(Rectangle &rect, const Vector2 pos)
{
    rect.x = pos.x;
    rect.y = pos.y;
}

inline Rectangle GetCentereredRect(const Rectangle *rect, const Vector2 pos)
{
    Rectangle newRect{};
    newRect.x = pos.x - rect->width / 2;
    newRect.y = pos.y - rect->height / 2;
    return newRect;
}

inline void MoveRect(Rectangle &rect, const Vector2 pos)
{
    rect.x += pos.x;
    rect.y += pos.y;
}
