#pragma once
#include <raylib.h>
#include <raymath.h>
#include "settings.h"

inline Vector2 GetRectCenter(const Rectangle rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

inline Vector2 GetRectMidBottom(const Rectangle rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height};
}

inline Vector2 GetRectMidTop(const Rectangle rect)
{
    return {rect.x + rect.width / 2, rect.y};
}

inline Vector2 GetRectMidLeft(const Rectangle rect)
{
    return {rect.x, rect.y + rect.height / 2};
}

inline Vector2 GetRectMidRight(const Rectangle rect)
{
    return {rect.x + rect.width, rect.y + rect.height / 2};
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


// ************* Adapted from Raylib "rshapes.c" ***************

// Error rate to calculate how many segments we need to draw a smooth circle,
// taken from https://stackoverflow.com/a/2244088
#ifndef SMOOTH_CIRCLE_ERROR_RATE
#define SMOOTH_CIRCLE_ERROR_RATE 0.5f // Circle error rate
#endif

// Draw rectangle with rounded edges
inline void DrawRectangleRoundedCorners(
        Rectangle rec, float roundness, int segments, const Color color, const bool TopLeft = true,
        const bool TopRight = true, const bool BottomRight = true, const bool BottomLeft = true)
{
    // Not a rounded rectangle
    if ((roundness <= 0.0f) || (rec.width < 1) || (rec.height < 1))
    {
        DrawRectangleRec(rec, color);
        return;
    }

    if (roundness >= 1.0f)
        roundness = 1.0f;

    // Calculate corner radius
    const float radius = (rec.width > rec.height) ? (rec.height * roundness) / 2 : (rec.width * roundness) / 2;
    if (radius <= 0.0f)
        return;

    // Calculate number of segments to use for the corners
    if (segments < 4)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        const float th = acosf(2 * powf(1 - SMOOTH_CIRCLE_ERROR_RATE / radius, 2) - 1);
        segments = (int) (ceilf(2 * PI / th) / 4.0f);
        if (segments <= 0)
            segments = 4;
    }

    // the roundness reduces height by 1 pixel, we correct it
    rec.height++;

    if (TopLeft)
    {
        DrawCircleSector({rec.x + radius, rec.y + radius}, radius, 180.0f, 270.0f, segments, color);
    }
    else
    {
        DrawRectangle(rec.x, rec.y, radius + 1, radius, color);
    }

    if (TopRight)
    {
        DrawCircleSector({rec.x + rec.width - radius, rec.y + radius}, radius, 270.0f, 360.0f, segments, color);
    }
    else
    {
        DrawRectangle(rec.x + rec.width - radius, rec.y, radius, radius, color);
    }

    if (BottomRight)
    {
        DrawCircleSector(
                {rec.x + rec.width - radius, rec.y + rec.height - radius}, radius, 0.0f, 90.0f, segments, color);
    }
    else
    {
        DrawRectangle(rec.x + rec.width - radius, rec.y + rec.height - radius, radius, radius, color);
    }

    if (BottomLeft)
    {
        DrawCircleSector({rec.x + radius, rec.y + rec.height - radius}, radius, 90.0f, 180.0f, segments, color);
    }
    else
    {
        DrawRectangle(rec.x, rec.y + rec.height - radius, radius, radius, color);
    }

    // the roundness removes one pixel from center-to-right, we add it
    // Top Rectangle
    DrawRectangle(rec.x + radius, rec.y, rec.width - 2 * radius + 1, radius, color);
    // Right Rectangle
    DrawRectangle(rec.x + rec.width - radius, rec.y + radius, radius, rec.height - 2 * radius + 1, color);
    // Bottom Rectangle
    DrawRectangle(rec.x + radius - 1, rec.y + rec.height - radius, rec.width - 2 * radius + 2, radius, color);
    // Left Rectangle
    DrawRectangle(rec.x, rec.y + radius, radius + 1, rec.height - 2 * radius + 1, color);
    // Center Rectangle
    DrawRectangle(rec.x + radius, rec.y + radius, rec.width - 2 * radius + 1, rec.height - 2 * radius + 1, color);
}
