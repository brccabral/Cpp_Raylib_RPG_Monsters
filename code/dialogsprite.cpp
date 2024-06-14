#include "dialogsprite.h"
#include <raymath.h>
#include "raylib_utils.h"

#include <iostream>
#include <utility>


DialogSprite::DialogSprite(
        std::string msg, const Character *trainer, const std::vector<SpriteGroup *> &grps, const Font &font_)
    : SimpleSprite(grps), message(std::move(msg)), font(font_)
{
    z = WORLD_LAYERS["top"];
    type = DIALOGSPRITE;

    constexpr float padding = 10.0f;
    // rect.width = std::max(TextLength(message.c_str()) * 16 + padding * 2, 300ul);
    textsize = MeasureTextEx(font, message.c_str(), FONT_SIZE, 2);
    rect.width = std::max(textsize.x + padding * 2, 300.0f);
    rect.height = textsize.y + padding * 2;
    // dialog midbottom to trainer midtop
    const auto [x, y] = GetRectMidtop(trainer->rect);
    rect.x = x - rect.width / 2;
    rect.y = y - rect.height - 10;

    y_sort = GetRectCenter(rect).y;

    image.rect = {0, 0, rect.width, rect.height};
    image.texture = CreateImage();
}

DialogSprite::~DialogSprite()
{
    for (auto *group: groups)
    {
        for (auto it = group->sprites.begin(); it != group->sprites.end();)
        {
            if (*it == this)
            {
                group->sprites.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

Texture2D *DialogSprite::CreateImage() const
{
    const RenderTexture2D render = LoadRenderTexture(image.rect.width, image.rect.height);
    while (!IsRenderTextureReady(render))
    {}
    BeginTextureMode(render);

    DrawRectangleRounded(image.rect, 0.3, 10, COLORS["pure white"]);

    // centralize text inside box
    const auto offset = Vector2Subtract({image.rect.width, image.rect.height}, textsize);
    DrawTextEx(font, message.c_str(), Vector2Scale(offset, 0.5f), FONT_SIZE, 2, COLORS["black"]);

    EndTextureMode();

    // need another render to invert the image
    // https://github.com/raysan5/raylib/issues/3803
    // https://github.com/raysan5/raylib/issues/378
    const RenderTexture2D inverted = LoadRenderTexture(image.rect.width, image.rect.height);
    while (!IsRenderTextureReady(inverted))
    {}

    BeginTextureMode(inverted);
    DrawTextureRec(render.texture, image.rect, {0, 0}, WHITE);
    EndTextureMode();

    const auto result = (Texture2D *) MemAlloc(sizeof(Texture2D));
    *result = inverted.texture;
    return result;
}
