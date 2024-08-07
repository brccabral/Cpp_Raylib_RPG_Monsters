#include <utility>
#include <algorithm>
#include "raylib_utils.h"
#include "dialogsprite.h"


DialogSprite::DialogSprite(
        std::string msg, const Character *trainer, const std::vector<SpriteGroup *> &grps,
        const Font &font_)
    : SimpleSprite(grps), message(std::move(msg)), font(font_)
{
    z = WORLD_LAYERS["top"];
    type = DIALOGSPRITE;

    constexpr float padding = 10.0f;
    textsize = MeasureTextF(font, message.c_str(), 2);
    rect.width = std::max(textsize.x + padding * 2, 300.0f);
    rect.height = textsize.y + padding * 2;
    // dialog midbottom to trainer midtop
    const auto [x, y] = GetRectMidTop(trainer->rect);
    rect.x = x - rect.width / 2;
    rect.y = y - rect.height - 10;

    y_sort = GetRectCenter(rect).y;

    image.rect = {0, 0, rect.size};
    image.texture = CreateImage();
}

DialogSprite::~DialogSprite()
{
    UnloadRenderTexture(render);
    // remove itself from groups so that groups.update() won't
    // call this object
    for (auto *group: groups)
    {
        group->sprites.erase(
                std::remove(group->sprites.begin(), group->sprites.end(), this),
                group->sprites.end());
    }
}

Texture2D *DialogSprite::CreateImage()
{
    // we Unload in ~DialogSprite()
    const RenderTexture2D inverted = LoadRenderTextureV(image.rect.size);
    BeginTextureModeC(inverted, BLANK);

    DrawRectangleRounded(image.rect.rectangle, 0.3f, 10, COLORS["pure white"]);

    // centralize text inside box
    const auto offset = Vector2Subtract(image.rect.size, textsize);
    DrawTextEx(font, message.c_str(), Vector2Scale(offset, 0.5f), FONT_SIZE, 2, COLORS["black"]);

    EndTextureModeSafe();

    // need another render to invert the image
    // https://github.com/raysan5/raylib/issues/3803
    // https://github.com/raysan5/raylib/issues/378
    // we unload in ~DialogSprite()
    render = LoadRenderTextureV(image.rect.size);

    BeginTextureModeC(render, BLANK);
    DrawTextureRec(inverted.texture, image.rect.rectangle, {0, 0}, WHITE);
    EndTextureModeSafe();

    UnloadRenderTexture(inverted);
    return &render.texture;
}
