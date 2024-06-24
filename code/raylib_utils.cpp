#include <random>
#include "raylib_utils.h"
#include "rlgl.h" // OpenGL abstraction layer to OpenGL 1.1, 2.1, 3.3+ or ES2
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


// ************* Adapted from Raylib "rshapes.c" ***************

// Error rate to calculate how many segments we need to draw a smooth circle,
// taken from https://stackoverflow.com/a/2244088
#ifndef SMOOTH_CIRCLE_ERROR_RATE
#define SMOOTH_CIRCLE_ERROR_RATE 0.5f // Circle error rate
#endif

extern Texture2D texShapes; // Texture used on shapes drawing (white pixel loaded by rlgl)
extern Rectangle texShapesRec; // Texture source rectangle used on shapes drawing

#if defined(SUPPORT_QUADS_DRAW_MODE)
void rlRectangle(
        const Vector2 p1, const Vector2 p2, const Vector2 p3, const Vector2 p4, const Color color,
        const Rectangle shapeRect, const Texture2D &texShapes)
{
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlTexCoord2f(shapeRect.x / texShapes.width, shapeRect.y / texShapes.height);
    rlVertex2f(p1.x, p1.y);
    rlTexCoord2f(
            shapeRect.x / texShapes.width, (shapeRect.y + shapeRect.height) / texShapes.height);
    rlVertex2f(p2.x, p2.y);
    rlTexCoord2f(
            (shapeRect.x + shapeRect.width) / texShapes.width,
            (shapeRect.y + shapeRect.height) / texShapes.height);
    rlVertex2f(p3.x, p3.y);
    rlTexCoord2f((shapeRect.x + shapeRect.width) / texShapes.width, shapeRect.y / texShapes.height);
    rlVertex2f(p4.x, p4.y);
}
#else
void rlRectangle(
        const Vector2 p1, const Vector2 p2, const Vector2 p3, const Vector2 p4, const Color color)
{
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex2f(p1.x, p1.y);
    rlVertex2f(p2.x, p2.y);
    rlVertex2f(p3.x, p3.y);
    rlVertex2f(p4.x, p4.y);
    rlVertex2f(p1.x, p1.y);
    rlVertex2f(p3.x, p3.y);
}
#endif

// Draw rectangle with rounded edges on selected corners
void DrawRectangleRoundedPro(
        const RectangleU rec, float roundness, int segments, const Color color, const bool topLeft,
        const bool topRight, const bool bottomLeft, const bool bottomRight)
{
    // Not a rounded rectangle
    if ((roundness <= 0.0f) || (rec.width < 1) || (rec.height < 1) ||
        (!topLeft && !topRight && !bottomLeft && !bottomRight))
    {
        DrawRectangleRec(rec.rectangle, color);
        return;
    }

    if (roundness >= 1.0f)
        roundness = 1.0f;

    // Calculate corner radius
    float radius =
            (rec.width > rec.height) ? (rec.height * roundness) / 2 : (rec.width * roundness) / 2;
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

    float stepLength = 90.0f / (float) segments;

    /*
    Quick sketch to make sense of all of this,
    there are 9 parts to draw, also mark the 12 points we'll use

          P0____________________P1
          /|                    |\
         /1|          2         |3\
     P7 /__|____________________|__\ P2
       |   |P8                P9|   |
       | 8 |          9         | 4 |
       | __|____________________|__ |
     P6 \  |P11              P10|  / P3
         \7|          6         |5/
          \|____________________|/
          P5                    P4
    */
    // Coordinates of the 12 points that define the rounded rect
    const Vector2 point[12] = {
            {rec.x + radius, rec.y},
            {(rec.x + rec.width) - radius, rec.y},
            {rec.x + rec.width, rec.y + radius}, // PO, P1, P2
            {rec.x + rec.width, (rec.y + rec.height) - radius},
            {(rec.x + rec.width) - radius, rec.y + rec.height}, // P3, P4
            {rec.x + radius, rec.y + rec.height},
            {rec.x, (rec.y + rec.height) - radius},
            {rec.x, rec.y + radius}, // P5, P6, P7
            {rec.x + radius, rec.y + radius},
            {(rec.x + rec.width) - radius, rec.y + radius}, // P8, P9
            {(rec.x + rec.width) - radius, (rec.y + rec.height) - radius},
            {rec.x + radius, (float) (rec.y + rec.height) - radius} // P10, P11
    };

    Vector2 centers[4];
    float angles[4];
    int countCenters = 0;
    if (topLeft)
    {
        centers[countCenters] = point[8];
        angles[countCenters] = 180.0f;
        ++countCenters;
    }
    if (topRight)
    {
        centers[countCenters] = point[9];
        angles[countCenters] = 270.0f;
        ++countCenters;
    }
    if (bottomRight)
    {
        centers[countCenters] = point[10];
        angles[countCenters] = 0.0f;
        ++countCenters;
    }
    if (bottomLeft)
    {
        centers[countCenters] = point[11];
        angles[countCenters] = 90.0f;
        ++countCenters;
    }

#if defined(SUPPORT_QUADS_DRAW_MODE)
    rlSetTexture(GetShapesTexture().id);
    const Rectangle shapeRect = GetShapesTextureRectangle();

    rlBegin(RL_QUADS);
    // Draw the selected corners
    for (int k = 0; k < countCenters; ++k) // Hope the compiler is smart enough to unroll this loop
    {
        float angle = angles[k];
        const Vector2 center = centers[k];

        // NOTE: Every QUAD actually represents two segments
        for (int i = 0; i < segments / 2; i++)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f(shapeRect.x / texShapes.width, shapeRect.y / texShapes.height);
            rlVertex2f(center.x, center.y);

            rlTexCoord2f(
                    (shapeRect.x + shapeRect.width) / texShapes.width,
                    shapeRect.y / texShapes.height);
            rlVertex2f(
                    center.x + cosf(DEG2RAD * (angle + stepLength * 2)) * radius,
                    center.y + sinf(DEG2RAD * (angle + stepLength * 2)) * radius);

            rlTexCoord2f(
                    (shapeRect.x + shapeRect.width) / texShapes.width,
                    (shapeRect.y + shapeRect.height) / texShapes.height);
            rlVertex2f(
                    center.x + cosf(DEG2RAD * (angle + stepLength)) * radius,
                    center.y + sinf(DEG2RAD * (angle + stepLength)) * radius);

            rlTexCoord2f(
                    shapeRect.x / texShapes.width,
                    (shapeRect.y + shapeRect.height) / texShapes.height);
            rlVertex2f(
                    center.x + cosf(DEG2RAD * angle) * radius,
                    center.y + sinf(DEG2RAD * angle) * radius);

            angle += (stepLength * 2);
        }

        // NOTE: In case number of segments is odd, we add one last piece to the cake
        if (segments % 2)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f(shapeRect.x / texShapes.width, shapeRect.y / texShapes.height);
            rlVertex2f(center.x, center.y);

            rlTexCoord2f(
                    (shapeRect.x + shapeRect.width) / texShapes.width,
                    (shapeRect.y + shapeRect.height) / texShapes.height);
            rlVertex2f(
                    center.x + cosf(DEG2RAD * (angle + stepLength)) * radius,
                    center.y + sinf(DEG2RAD * (angle + stepLength)) * radius);

            rlTexCoord2f(
                    shapeRect.x / texShapes.width,
                    (shapeRect.y + shapeRect.height) / texShapes.height);
            rlVertex2f(
                    center.x + cosf(DEG2RAD * angle) * radius,
                    center.y + sinf(DEG2RAD * angle) * radius);

            rlTexCoord2f(
                    (shapeRect.x + shapeRect.width) / texShapes.width,
                    shapeRect.y / texShapes.height);
            rlVertex2f(center.x, center.y);
        }
    }

    if (!topLeft)
    {
        rlRectangle(
                (Vector2){point[7].x, point[0].y}, point[7], point[8], point[0], color, shapeRect,
                texShapes);
    }
    if (!topRight)
    {
        rlRectangle(
                point[1], point[9], point[2], (Vector2){point[2].x, point[1].y}, color, shapeRect,
                texShapes);
    }
    if (!bottomLeft)
    {
        rlRectangle(
                point[6], (Vector2){point[6].x, point[5].y}, point[5], point[11], color, shapeRect,
                texShapes);
    }
    if (!bottomRight)
    {
        rlRectangle(
                point[10], point[4], (Vector2){point[3].x, point[4].y}, point[3], color, shapeRect,
                texShapes);
    }

    // [2] Upper Rectangle
    rlRectangle(point[0], point[8], point[9], point[1], color, shapeRect, texShapes);

    // [4] Right Rectangle
    rlRectangle(point[9], point[10], point[3], point[2], color, shapeRect, texShapes);

    // [6] Bottom Rectangle
    rlRectangle(point[11], point[5], point[4], point[10], color, shapeRect, texShapes);

    // [8] Left Rectangle
    rlRectangle(point[7], point[6], point[11], point[8], color, shapeRect, texShapes);

    // [9] Middle Rectangle
    rlRectangle(point[8], point[11], point[10], point[9], color, shapeRect, texShapes);

    rlEnd();
    rlSetTexture(0);

#else
    rlBegin(RL_TRIANGLES);

    // Draw the selected corners
    for (int k = 0; k < countCenters; ++k) // Hope the compiler is smart enough to unroll this loop
    {
        float angle = angles[k];
        const Vector2 center = centers[k];
        for (int i = 0; i < segments; i++)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlVertex2f(center.x, center.y);
            rlVertex2f(
                    center.x + cosf(DEG2RAD * (angle + stepLength)) * radius,
                    center.y + sinf(DEG2RAD * (angle + stepLength)) * radius);
            rlVertex2f(
                    center.x + cosf(DEG2RAD * angle) * radius,
                    center.y + sinf(DEG2RAD * angle) * radius);
            angle += stepLength;
        }
    }

    if (!topLeft)
    {
        rlRectangle((Vector2){point[7].x, point[0].y}, point[7], point[8], point[0], color);
    }
    if (!topRight)
    {
        rlRectangle(point[1], point[9], point[2], (Vector2){point[2].x, point[1].y}, color);
    }
    if (!bottomLeft)
    {
        rlRectangle(point[6], (Vector2){point[6].x, point[5].y}, point[5], point[11], color);
    }
    if (!bottomRight)
    {
        rlRectangle(point[10], point[4], (Vector2){point[3].x, point[4].y}, point[3], color);
    }

    // [2] Upper Rectangle
    rlRectangle(point[0], point[8], point[9], point[1], color);

    // [4] Right Rectangle
    rlRectangle(point[9], point[10], point[3], point[2], color);

    // [6] Bottom Rectangle
    rlRectangle(point[11], point[5], point[4], point[10], color);

    // [8] Left Rectangle
    rlRectangle(point[7], point[6], point[11], point[8], color);

    // [9] Middle Rectangle
    rlRectangle(point[8], point[11], point[10], point[9], color);

    rlEnd();

#endif
}
