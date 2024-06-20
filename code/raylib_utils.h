#pragma once
#include <raylib.h>
#include <raymath.h>

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

RLAPI Vector2 GetRectCenter(RectangleU rect);

RLAPI Vector2 GetRectMidBottom(RectangleU rect);

RLAPI Vector2 GetRectMidTop(RectangleU rect);

RLAPI Vector2 GetRectMidLeft(RectangleU rect);

RLAPI Vector2 GetRectMidRight(RectangleU rect);

RLAPI Vector2 GetRectTopLeft(RectangleU rect);

RLAPI Vector2 GetRectTopRight(RectangleU rect);

RLAPI Vector2 GetRectBottomLeft(RectangleU rect);

RLAPI Vector2 GetRectBottomRight(RectangleU rect);

RLAPI void RectToCenter(RectangleU &rect, Vector2 pos);

RLAPI void RectToMidBottom(RectangleU &rect, Vector2 pos);

RLAPI void RectToMidLeft(RectangleU &rect, Vector2 pos);

RLAPI void RectToBottomLeft(RectangleU &rect, Vector2 pos);

RLAPI void RectToTopLeft(RectangleU &rect, Vector2 pos);

RLAPI void RectToTopRight(RectangleU &rect, Vector2 pos);

RLAPI Vector2 ViewOffset(Vector2 player_center);

RLAPI RectangleU CreateCenteredRect(RectangleU rect, Vector2 pos);

RLAPI void MoveVector2(Vector2 &pos, Vector2 delta);

RLAPI void MoveRect(RectangleU &rect, Vector2 pos);

RLAPI void RectInflate(RectangleU &rect, float width, float height);

RLAPI void AbsRect(RectangleU &rect);

RLAPI bool CheckCollisionRectLine(
        RectangleU rect, Vector2 startPos, Vector2 endPos, Vector2 *collisionPoint1,
        Vector2 *collisionPoint2);

// Draw rectangle with rounded edges
RLAPI void DrawRectangleRoundedCorners(
        RectangleU rec, float roundness, int segments, Color color, bool TopLeft = true,
        bool TopRight = true, bool BottomRight = true, bool BottomLeft = true);

RLAPI Vector2 MeasureTextF(const Font &font, const char *text, int spacing);

RLAPI RenderTexture2D LoadRenderTextureV(Vector2 size);

void BeginTextureModeSafe(const RenderTexture2D &render);

void EndTextureModeSafe();

RLAPI void BeginTextureModeC(const RenderTexture2D &render, Color color);

RLAPI void BeginDrawingC(Color color);

RLAPI float GetRandomUniform(float min, float max);

RLAPI void DrawCenteredTextEx(const Font &font, const char *text, RectangleU rect, Color color);
