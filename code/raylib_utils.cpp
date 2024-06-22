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

std::uniform_real_distribution<float> GetRandomUniformDist(const float min, const float max)
{
    const std::uniform_real_distribution<float> dist(min, max);
    return dist;
}

float GetRandomUniform(std::uniform_real_distribution<float> dist)
{
    return dist(gen);
}

void DrawCenteredTextEx(
        const Font &font, const char *text, const RectangleU rect, const Color color)
{
    const auto [text_width, text_height] = MeasureTextEx(font, text, font.baseSize, 1);
    const Vector2 text_pos = Vector2Add(GetRectCenter(rect), {-text_width / 2, -text_height / 2});
    DrawTextEx(font, text, text_pos, font.baseSize, 1, color);
}

void DrawBar(
        const RectangleU rect, const float value, const float max_value, const Color color,
        const Color bg_color, const float roundness, const int segments)
{
    const float ratio = rect.width / max_value;
    const RectangleU bg_rect = rect;
    const RectangleU progress_rect = {
            rect.x, rect.y, Clamp(value * ratio, 0, rect.width), rect.height};

    if (roundness == 0)
    {
        DrawRectangleRec(bg_rect.rectangle, bg_color);
        DrawRectangleRec(progress_rect.rectangle, color);
    }
    else
    {
        DrawRectangleRounded(bg_rect.rectangle, roundness, segments, bg_color);
        DrawRectangleRounded(progress_rect.rectangle, roundness, segments, color);
    }
}

Texture2D *Texture2DToPointer(const Texture2D &texture)
{
    auto *returnValue = (Texture2D *) MemAlloc(sizeof(Texture2D));
    *returnValue = texture;
    return returnValue;
}

// Convert half-float (stored as unsigned short) to float
// REF:
// https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion/60047308#60047308
static float HalfToFloat(const unsigned short x)
{

    const unsigned int e = (x & 0x7C00) >> 10; // Exponent
    const unsigned int m = (x & 0x03FF) << 13; // Mantissa
    const auto fm = (float) m;
    const unsigned int v = (*(unsigned int *) &fm) >>
                           23; // Evil log2 bit hack to count leading zeros in denormalized format
    const unsigned int r =
            (x & 0x8000) << 16 | (e != 0) * ((e + 112) << 23 | m) |
            ((e == 0) & (m != 0)) *
                    ((v - 37) << 23 |
                     ((m << (150 - v)) & 0x007FE000)); // sign : normalized : denormalized

    const float result = *(float *) &r;

    return result;
}

// Get pixel data from image as Vector4 array (float normalized)
static Vector4 *LoadImageDataNormalized(const Image &image)
{
    auto *pixels = (Vector4 *) RL_MALLOC(image.width * image.height * sizeof(Vector4));

    if (image.format >= PIXELFORMAT_COMPRESSED_DXT1_RGB)
        TraceLog(
                LOG_WARNING,
                "IMAGE: Pixel data retrieval not supported for compressed image formats");
    else
    {
        for (int i = 0, k = 0; i < image.width * image.height; i++)
        {
            switch (image.format)
            {
                case PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
                {
                    pixels[i].x = (float) ((unsigned char *) image.data)[i] / 255.0f;
                    pixels[i].y = (float) ((unsigned char *) image.data)[i] / 255.0f;
                    pixels[i].z = (float) ((unsigned char *) image.data)[i] / 255.0f;
                    pixels[i].w = 1.0f;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
                {
                    pixels[i].x = (float) ((unsigned char *) image.data)[k] / 255.0f;
                    pixels[i].y = (float) ((unsigned char *) image.data)[k] / 255.0f;
                    pixels[i].z = (float) ((unsigned char *) image.data)[k] / 255.0f;
                    pixels[i].w = (float) ((unsigned char *) image.data)[k + 1] / 255.0f;

                    k += 2;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:
                {
                    const unsigned short pixel = ((unsigned short *) image.data)[i];

                    pixels[i].x = (float) ((pixel & 0b1111100000000000) >> 11) * (1.0f / 31);
                    pixels[i].y = (float) ((pixel & 0b0000011111000000) >> 6) * (1.0f / 31);
                    pixels[i].z = (float) ((pixel & 0b0000000000111110) >> 1) * (1.0f / 31);
                    pixels[i].w = ((pixel & 0b0000000000000001) == 0) ? 0.0f : 1.0f;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R5G6B5:
                {
                    const unsigned short pixel = ((unsigned short *) image.data)[i];

                    pixels[i].x = (float) ((pixel & 0b1111100000000000) >> 11) * (1.0f / 31);
                    pixels[i].y = (float) ((pixel & 0b0000011111100000) >> 5) * (1.0f / 63);
                    pixels[i].z = (float) (pixel & 0b0000000000011111) * (1.0f / 31);
                    pixels[i].w = 1.0f;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:
                {
                    const unsigned short pixel = ((unsigned short *) image.data)[i];

                    pixels[i].x = (float) ((pixel & 0b1111000000000000) >> 12) * (1.0f / 15);
                    pixels[i].y = (float) ((pixel & 0b0000111100000000) >> 8) * (1.0f / 15);
                    pixels[i].z = (float) ((pixel & 0b0000000011110000) >> 4) * (1.0f / 15);
                    pixels[i].w = (float) (pixel & 0b0000000000001111) * (1.0f / 15);
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
                {
                    pixels[i].x = (float) ((unsigned char *) image.data)[k] / 255.0f;
                    pixels[i].y = (float) ((unsigned char *) image.data)[k + 1] / 255.0f;
                    pixels[i].z = (float) ((unsigned char *) image.data)[k + 2] / 255.0f;
                    pixels[i].w = (float) ((unsigned char *) image.data)[k + 3] / 255.0f;

                    k += 4;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R8G8B8:
                {
                    pixels[i].x = (float) ((unsigned char *) image.data)[k] / 255.0f;
                    pixels[i].y = (float) ((unsigned char *) image.data)[k + 1] / 255.0f;
                    pixels[i].z = (float) ((unsigned char *) image.data)[k + 2] / 255.0f;
                    pixels[i].w = 1.0f;

                    k += 3;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R32:
                {
                    pixels[i].x = ((float *) image.data)[k];
                    pixels[i].y = 0.0f;
                    pixels[i].z = 0.0f;
                    pixels[i].w = 1.0f;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R32G32B32:
                {
                    pixels[i].x = ((float *) image.data)[k];
                    pixels[i].y = ((float *) image.data)[k + 1];
                    pixels[i].z = ((float *) image.data)[k + 2];
                    pixels[i].w = 1.0f;

                    k += 3;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R32G32B32A32:
                {
                    pixels[i].x = ((float *) image.data)[k];
                    pixels[i].y = ((float *) image.data)[k + 1];
                    pixels[i].z = ((float *) image.data)[k + 2];
                    pixels[i].w = ((float *) image.data)[k + 3];

                    k += 4;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R16:
                {
                    pixels[i].x = HalfToFloat(((unsigned short *) image.data)[k]);
                    pixels[i].y = 0.0f;
                    pixels[i].z = 0.0f;
                    pixels[i].w = 1.0f;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R16G16B16:
                {
                    pixels[i].x = HalfToFloat(((unsigned short *) image.data)[k]);
                    pixels[i].y = HalfToFloat(((unsigned short *) image.data)[k + 1]);
                    pixels[i].z = HalfToFloat(((unsigned short *) image.data)[k + 2]);
                    pixels[i].w = 1.0f;

                    k += 3;
                }
                break;
                case PIXELFORMAT_UNCOMPRESSED_R16G16B16A16:
                {
                    pixels[i].x = HalfToFloat(((unsigned short *) image.data)[k]);
                    pixels[i].y = HalfToFloat(((unsigned short *) image.data)[k + 1]);
                    pixels[i].z = HalfToFloat(((unsigned short *) image.data)[k + 2]);
                    pixels[i].w = HalfToFloat(((unsigned short *) image.data)[k + 3]);

                    k += 4;
                }
                break;
                default:
                    break;
            }
        }
    }

    return pixels;
}

Image ImageMaskFromImage(const Image &image, const Color color, const float threshold)
{
    // Security check to avoid program crash
    if ((image.data == nullptr) || (image.width == 0) || (image.height == 0))
        return {};

    Vector4 *pixels = LoadImageDataNormalized(image);

    Image mask{};
    mask.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    mask.height = image.height;
    mask.width = image.width;
    mask.mipmaps = 1;

    auto *new_pixels = (Color *) RL_CALLOC(image.width * image.height, sizeof(Color));

    for (int i = 0; i < mask.width * mask.height; i++)
    {
        if (pixels[i].w > threshold)
            new_pixels[i] = color;
        else
            new_pixels[i] = {};
    }

    mask.data = new_pixels;

    RL_FREE(pixels);
    pixels = nullptr;

    return mask;
}

Texture TextureMaskFromTexture(const Texture2D *texture, const Color color, const float threshold)
{
    const Image image = LoadImageFromTexture(*texture);
    const Image mask = ImageMaskFromImage(image, color, threshold);
    const Texture result = LoadTextureFromImage(mask);
    UnloadImage(image);
    UnloadImage(mask);
    return result;
}

Image GenImageRandomPixels(const float width, const float height)
{
    Image image{};
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    image.height = height;
    image.width = width;
    image.mipmaps = 1;

    auto *pixels = (Color *) RL_CALLOC(width * height, sizeof(Color));
    for (int i = 0; i < width * height; i++)
    {
        pixels[i].r = GetRandomValue(0, 255);
        pixels[i].g = GetRandomValue(0, 255);
        pixels[i].b = GetRandomValue(0, 255);
        pixels[i].a = GetRandomValue(0, 255);
    }
    image.data = pixels;

    return image;
}

Texture2D TextureColorGrayscale(const Texture2D *texture)
{
    Image image = LoadImageFromTexture(*texture);
    ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA);
    const Texture result = LoadTextureFromImage(image);
    UnloadImage(image);
    return result;
}
