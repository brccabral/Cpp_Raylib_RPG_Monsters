#pragma once
#include <raylib.h>
#include <raymath.h>
#include "settings.h"

inline Vector2 GetRectCenter(const Rectangle rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

inline Vector2 GetRectMidbottom(const Rectangle rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height};
}

inline Vector2 GetRectMidtop(const Rectangle rect)
{
    return {rect.x + rect.width / 2, rect.y};
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

inline Vector2 ViewOffset(const Vector2 player_center)
{
    Vector2 offset{};
    offset = Vector2Subtract(player_center, Vector2Scale({WINDOW_WIDTH, WINDOW_HEIGHT}, 0.5));
    offset = Vector2Scale(offset, -1);
    return offset;
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

inline void RectInflate(Rectangle &rect, const float width, const float height)
{
    const Vector2 oldCenter = GetRectCenter(rect);
    rect.width += width;
    rect.height += height;
    RectToCenter(rect, oldCenter);
}
