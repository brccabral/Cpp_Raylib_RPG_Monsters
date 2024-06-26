#pragma once
#include <raylib.h>
#include <raymath.h>
#include <random>

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

RLAPI void DrawRectangleRoundedEx(
        RectangleU rec, float roundness, int segments, Color color, bool topLeft = true,
        bool topRight = true, bool bottomLeft = true, bool bottomRight = true);

RLAPI Vector2 MeasureTextF(const Font &font, const char *text, int spacing);

RLAPI RenderTexture2D LoadRenderTextureV(Vector2 size);

void BeginTextureModeSafe(const RenderTexture2D &render);

void EndTextureModeSafe();

RLAPI void BeginTextureModeC(const RenderTexture2D &render, Color color);

RLAPI void BeginDrawingC(Color color);

RLAPI std::uniform_real_distribution<float> GetRandomUniformDist(float min, float max);

RLAPI float GetRandomUniform(std::uniform_real_distribution<float> dist);

RLAPI void DrawCenteredTextEx(const Font &font, const char *text, RectangleU rect, Color color);

RLAPI void
DrawBar(RectangleU rect, float value, float max_value, Color color, Color bg_color,
        float roundness = 0.0f, int segments = 0);

RLAPI Texture2D *Texture2DToPointer(const Texture2D &texture);

RLAPI Image ImageFromChannel(const Image &image, int selected_channel, float threshold);

RLAPI Texture2D TextureMaskFromTexture(const Texture2D *texture, Color color, float threshold = 0);

RLAPI Image GenImageRandomPixels(float width, float height);

RLAPI Texture2D TextureColorGrayscale(const Texture2D *texture);
