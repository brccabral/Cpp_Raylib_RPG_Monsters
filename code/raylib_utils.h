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

inline Vector2 GetRectTopLeft(const Rectangle rect)
{
    return {rect.x, rect.y};
}

inline Vector2 GetRectTopRight(const Rectangle rect)
{
    return {rect.x + rect.width, rect.y};
}

inline Vector2 GetRectBottomLeft(const Rectangle rect)
{
    return {rect.x, rect.y + rect.height};
}

inline Vector2 GetRectBottomRight(const Rectangle rect)
{
    return {rect.x + rect.width, rect.y + rect.height};
}

inline void RectToCenter(Rectangle &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width / 2;
    rect.y = pos.y - rect.height / 2;
}

inline void RectToMidbottom(Rectangle &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width / 2;
    rect.y = pos.y - rect.height;
}

inline void RectToTopLeft(Rectangle &rect, const Vector2 pos)
{
    rect.x = pos.x;
    rect.y = pos.y;
}

inline Vector2 ViewOffset(const Vector2 player_center)
{
    Vector2 offset = Vector2Subtract(player_center, Vector2Scale({WINDOW_WIDTH, WINDOW_HEIGHT}, 0.5));
    offset = Vector2Scale(offset, -1);
    return offset;
}

inline Rectangle CreateCenteredRect(const Rectangle rect, const Vector2 pos)
{
    Rectangle newRect = rect;
    RectToCenter(newRect, pos);
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

inline void AbsRect(Rectangle &rect)
{
    if (rect.width < 0)
    {
        rect.x += rect.width;
        rect.width = -rect.width;
    }

    if (rect.height < 0)
    {
        rect.y += rect.height;
        rect.height = -rect.height;
    }
}

inline bool CheckCollisionRectLine(
        const Rectangle rect, const Vector2 startPos, const Vector2 endPos, Vector2 *collisionPoint1,
        Vector2 *collisionPoint2)
{
    bool hasCollision = false;
    bool secondCollision = false;
    Vector2 *collisionPoint = collisionPoint1;

    const Vector2 topLeft = GetRectTopLeft(rect);
    const Vector2 topRight = GetRectTopRight(rect);
    const Vector2 bottomLeft = GetRectBottomLeft(rect);
    const Vector2 bottomRight = GetRectBottomRight(rect);

    if (CheckCollisionLines(startPos, endPos, topLeft, topRight, collisionPoint))
    {
        collisionPoint = collisionPoint2;
        hasCollision = true;
    }
    if (CheckCollisionLines(startPos, endPos, bottomLeft, bottomRight, collisionPoint))
    {
        if (hasCollision)
        {
            secondCollision = true;
        }
        else
        {
            collisionPoint = collisionPoint2;
        }
        hasCollision = true;
    }
    if (!secondCollision && CheckCollisionLines(startPos, endPos, topLeft, bottomLeft, collisionPoint))
    {
        if (hasCollision)
        {
            secondCollision = true;
        }
        else
        {
            collisionPoint = collisionPoint2;
        }
        hasCollision = true;
    }
    if (!secondCollision && CheckCollisionLines(startPos, endPos, topRight, bottomRight, collisionPoint))
    {
        hasCollision = true;
    }

    return hasCollision;
}
