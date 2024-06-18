#pragma once
#include <raylib.h>
#include <raymath.h>
#include "settings.h"

typedef union RectangleU
{
    struct
    {
        Vector2 pos, size;
    };
    struct
    {
        Rectangle rectangle;
    };
    struct
    {
        float x, y, width, height;
    };
} RectangleU;

typedef union Vector2U
{
    struct
    {
        float x, y;
    };
    Vector2 vector2;
    float f[2];
} Vector2U;

typedef union Vector3U
{
    struct
    {
        float x, y, z;
    };
    struct
    {
        float r, g, b;
    };
    Vector2U xy;
    Vector3 vector3;
    float f[3];
} Vector3U;

typedef union Vector4U
{
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float r, g, b, a;
    };
    struct
    {
        Vector2 xy, zw;
    };
    Vector3U xyz;
    Vector3U rgb;
    Vector4 vector4;
    float f[4];
} Vector4U;

inline Vector2 GetRectCenter(const RectangleU rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

inline Vector2 GetRectMidBottom(const RectangleU rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height};
}

inline Vector2 GetRectMidTop(const RectangleU rect)
{
    return {rect.x + rect.width / 2, rect.y};
}

inline Vector2 GetRectMidLeft(const RectangleU rect)
{
    return {rect.x, rect.y + rect.height / 2};
}

inline Vector2 GetRectMidRight(const RectangleU rect)
{
    return {rect.x + rect.width, rect.y + rect.height / 2};
}

inline Vector2 GetRectTopLeft(const RectangleU rect)
{
    // return {rect.x, rect.y};
    return rect.pos;
}

inline Vector2 GetRectTopRight(const RectangleU rect)
{
    return {rect.x + rect.width, rect.y};
}

inline Vector2 GetRectBottomLeft(const RectangleU rect)
{
    return {rect.x, rect.y + rect.height};
}

inline Vector2 GetRectBottomRight(const RectangleU rect)
{
    return {rect.x + rect.width, rect.y + rect.height};
}

inline void RectToCenter(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width / 2;
    rect.y = pos.y - rect.height / 2;
}

inline void RectToMidBottom(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width / 2;
    rect.y = pos.y - rect.height;
}

inline void RectToMidLeft(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x;
    rect.y = pos.y - rect.height / 2;
}

inline void RectToBottomLeft(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x;
    rect.y = pos.y - rect.height;
}

inline void RectToTopLeft(RectangleU &rect, const Vector2 pos)
{
    // rect.x = pos.x;
    // rect.y = pos.y;
    rect.pos = pos;
}

inline Vector2 ViewOffset(const Vector2 player_center)
{
    Vector2 offset =
            Vector2Subtract(player_center, Vector2Scale({WINDOW_WIDTH, WINDOW_HEIGHT}, 0.5));
    offset = Vector2Scale(offset, -1);
    return offset;
}

inline RectangleU CreateCenteredRect(const RectangleU rect, const Vector2 pos)
{
    RectangleU newRect = rect;
    RectToCenter(newRect, pos);
    return newRect;
}

inline void MoveVector2(Vector2 &pos, const Vector2 delta)
{
    pos.x += delta.x;
    pos.y += delta.y;
}

inline void MoveRect(RectangleU &rect, const Vector2 pos)
{
    // rect.x += pos.x;
    // rect.y += pos.y;
    MoveVector2(rect.pos, pos);
}

inline void RectInflate(RectangleU &rect, const float width, const float height)
{
    const Vector2 oldCenter = GetRectCenter(rect);
    rect.width += width;
    rect.height += height;
    RectToCenter(rect, oldCenter);
}

inline void AbsRect(RectangleU &rect)
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
        const RectangleU rect, const Vector2 startPos, const Vector2 endPos,
        Vector2 *collisionPoint1, Vector2 *collisionPoint2)
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
    if (!secondCollision &&
        CheckCollisionLines(startPos, endPos, topLeft, bottomLeft, collisionPoint))
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
    if (!secondCollision &&
        CheckCollisionLines(startPos, endPos, topRight, bottomRight, collisionPoint))
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
        const RectangleU rec, float roundness, int segments, const Color color,
        const bool TopLeft = true, const bool TopRight = true, const bool BottomRight = true,
        const bool BottomLeft = true)
{
    // Not a rounded rectangle
    if ((roundness <= 0.0f) || (rec.width < 1) || (rec.height < 1))
    {
        DrawRectangleRec(rec.rectangle, color);
        return;
    }

    if (roundness >= 1.0f)
        roundness = 1.0f;

    // Calculate corner radius
    const float radius = ((rec.width + rec.height) / 2) * roundness / 2;
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

    // the roundness reduces some dimensions by 1 pixel, we correct it
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
        DrawCircleSector(
                {rec.x + rec.width - radius, rec.y + radius}, radius, 270.0f, 360.0f, segments,
                color);
    }
    else
    {
        DrawRectangle(rec.x + rec.width - radius, rec.y, radius + 1, radius, color);
    }

    if (BottomRight)
    {
        DrawCircleSector(
                {rec.x + rec.width - radius, rec.y + rec.height - radius}, radius, 0.0f, 90.0f,
                segments, color);
    }
    else
    {
        DrawRectangle(
                rec.x + rec.width - radius, rec.y + rec.height - radius, radius + 1, radius + 1,
                color);
    }

    if (BottomLeft)
    {
        DrawCircleSector(
                {rec.x + radius, rec.y + rec.height - radius}, radius, 90.0f, 180.0f, segments,
                color);
    }
    else
    {
        DrawRectangle(rec.x, rec.y + rec.height - radius, radius + 1, radius + 1, color);
    }

    // Top RectangleU
    DrawRectangle(rec.x + radius, rec.y, rec.width - 2 * radius + 1, radius, color);
    // Right RectangleU
    DrawRectangle(
            rec.x + rec.width - radius, rec.y + radius, radius + 1, rec.height - 2 * radius + 1,
            color);
    // Bottom RectangleU
    DrawRectangle(
            rec.x + radius - 1, rec.y + rec.height - radius, rec.width - 2 * radius + 2, radius + 1,
            color);
    // Left RectangleU
    DrawRectangle(rec.x, rec.y + radius, radius + 1, rec.height - 2 * radius + 1, color);
    // Center RectangleU
    DrawRectangle(
            rec.x + radius, rec.y + radius, rec.width - 2 * radius + 1, rec.height - 2 * radius + 1,
            color);
}

inline Vector2 MeasureTextF(const Font &font, const char *text, const int spacing)
{
    return MeasureTextEx(font, text, font.baseSize, spacing);
}

inline RenderTexture2D LoadRenderTextureV(const Vector2 size)
{
    return LoadRenderTexture(size.x, size.y);
}
