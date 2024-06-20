#include <random>
#include <chrono>
#include "raylib_utils.h"
#include "settings.h"


int current_render = 0;

void BeginTextureModeSafe(const RenderTexture2D &render)
{
    if (current_render > 0)
    {
        TraceLog(
                LOG_WARNING, TextFormat(
                                     "Double call to BeginTextureMode(), previous id %i new id %i",
                                     current_render, render.id));
    }
    current_render = render.id;
    BeginTextureMode(render);
}

void EndTextureModeSafe()
{
    current_render = 0;
    EndTextureMode();
}

Vector2 GetRectCenter(const RectangleU rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

Vector2 GetRectMidBottom(const RectangleU rect)
{
    return {rect.x + rect.width / 2, rect.y + rect.height};
}

Vector2 GetRectMidTop(const RectangleU rect)
{
    return {rect.x + rect.width / 2, rect.y};
}

Vector2 GetRectMidLeft(const RectangleU rect)
{
    return {rect.x, rect.y + rect.height / 2};
}

Vector2 GetRectMidRight(const RectangleU rect)
{
    return {rect.x + rect.width, rect.y + rect.height / 2};
}

Vector2 GetRectTopLeft(const RectangleU rect)
{
    // return {rect.x, rect.y};
    return rect.pos;
}

Vector2 GetRectTopRight(const RectangleU rect)
{
    return {rect.x + rect.width, rect.y};
}

Vector2 GetRectBottomLeft(const RectangleU rect)
{
    return {rect.x, rect.y + rect.height};
}

Vector2 GetRectBottomRight(const RectangleU rect)
{
    return {rect.x + rect.width, rect.y + rect.height};
}

void RectToCenter(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width / 2;
    rect.y = pos.y - rect.height / 2;
}

void RectToMidBottom(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width / 2;
    rect.y = pos.y - rect.height;
}

void RectToMidLeft(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x;
    rect.y = pos.y - rect.height / 2;
}

void RectToBottomLeft(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x;
    rect.y = pos.y - rect.height;
}

void RectToTopLeft(RectangleU &rect, const Vector2 pos)
{
    // rect.x = pos.x;
    // rect.y = pos.y;
    rect.pos = pos;
}

void RectToTopRight(RectangleU &rect, const Vector2 pos)
{
    rect.x = pos.x - rect.width;
    rect.y = pos.y;
}

Vector2 ViewOffset(const Vector2 player_center)
{
    Vector2 offset =
            Vector2Subtract(player_center, Vector2Scale({WINDOW_WIDTH, WINDOW_HEIGHT}, 0.5));
    offset = Vector2Scale(offset, -1);
    return offset;
}

RectangleU CreateCenteredRect(const RectangleU rect, const Vector2 pos)
{
    RectangleU newRect = rect;
    RectToCenter(newRect, pos);
    return newRect;
}

void MoveVector2(Vector2 &pos, const Vector2 delta)
{
    pos.x += delta.x;
    pos.y += delta.y;
}

void MoveRect(RectangleU &rect, const Vector2 pos)
{
    // rect.x += pos.x;
    // rect.y += pos.y;
    MoveVector2(rect.pos, pos);
}

void RectInflate(RectangleU &rect, const float width, const float height)
{
    const Vector2 oldCenter = GetRectCenter(rect);
    rect.width += width;
    rect.height += height;
    RectToCenter(rect, oldCenter);
}

void AbsRect(RectangleU &rect)
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

bool CheckCollisionRectLine(
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
RLAPI void DrawRectangleRoundedCorners(
        const RectangleU rec, float roundness, int segments, const Color color, const bool TopLeft,
        const bool TopRight, const bool BottomRight, const bool BottomLeft)
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
        DrawRectangle(rec.x, rec.y + rec.height - radius, radius, radius + 1, color);
    }

    // Top RectangleU
    DrawRectangle(rec.x + radius - 1, rec.y, rec.width - 2 * radius + 3, radius, color);
    // Right RectangleU
    DrawRectangle(
            rec.x + rec.width - radius, rec.y + radius - 1, radius + 1, rec.height - 2 * radius + 3,
            color);
    // Bottom RectangleU
    DrawRectangle(
            rec.x + radius - 1, rec.y + rec.height - radius, rec.width - 2 * radius + 3, radius + 1,
            color);
    // Left RectangleU
    DrawRectangle(rec.x, rec.y + radius - 1, radius, rec.height - 2 * radius + 3, color);
    // Center RectangleU
    DrawRectangle(
            rec.x + radius - 1, rec.y + radius - 1, rec.width - 2 * radius + 2,
            rec.height - 2 * radius + 2, color);
}

Vector2 MeasureTextF(const Font &font, const char *text, const int spacing)
{
    return MeasureTextEx(font, text, font.baseSize, spacing);
}

RenderTexture2D LoadRenderTextureV(const Vector2 size)
{
    const RenderTexture2D render = LoadRenderTexture(size.x, size.y);
    while (!IsRenderTextureReady(render))
    {}
    return render;
}

void BeginTextureModeC(const RenderTexture2D &render, const Color color)
{
    BeginTextureModeSafe(render);
    ClearBackground(color); // remove old memory
}

void BeginDrawingC(const Color color)
{
    BeginDrawing();
    ClearBackground(color);
}

std::random_device rd{};
std::mt19937 gen(rd());

float GetRandomUniform(const float min, const float max)
{
    std::uniform_real_distribution<> dist(min, max);
    return dist(gen);
}

void DrawCenteredTextEx(
        const Font &font, const char *text, const RectangleU rect, const Color color)
{
    const auto [text_width, text_height] = MeasureTextEx(font, text, font.baseSize, 1);
    const Vector2 text_pos = {(rect.width - text_width) / 2, (rect.height - text_height) / 2};
    DrawTextEx(font, text, text_pos, font.baseSize, 1, color);
}
