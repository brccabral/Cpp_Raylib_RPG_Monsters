#include "evolution.h"

Evolution::Evolution(
        std::map<std::string, Texture2D> *textures,
        std::map<std::string, animation_rects> *animation_frames, const std::string &start_monster,
        const std::string &end_monster, Font font, const std::function<void()> &end_evolution)
{
    Image start_image = LoadImageFromTexture((*textures)[start_monster]);
    ImageResize(&start_image, start_image.width * 2, start_image.height * 2);
    start_surf.texture = (Texture2D *) MemAlloc(sizeof(Texture2D));
    *(start_surf.texture) = LoadTextureFromImage(start_image);

    start_surf.rect = (*animation_frames)[start_monster][ANIMATION_IDLE][0];
    start_surf.rect.width *= 2;
    start_surf.rect.height *= 2;

    Image start_image_mask = ImageFromChannel(start_image, 3);
    ImageAlphaMask(&start_image_mask, start_image_mask);
    start_mask.texture = (Texture2D *) MemAlloc(sizeof(Texture2D));
    *(start_mask.texture) = LoadTextureFromImage(start_image_mask);

    start_mask.rect = start_surf.rect;

    UnloadImage(start_image);
    UnloadImage(start_image_mask);

    Image end_image = LoadImageFromTexture((*textures)[end_monster]);
    ImageResize(&end_image, end_image.width * 2, end_image.height * 2);
    end_surf.texture = (Texture2D *) MemAlloc(sizeof(Texture2D));
    *(end_surf.texture) = LoadTextureFromImage(end_image);

    end_surf.rect = (*animation_frames)[end_monster][ANIMATION_IDLE][0];
    end_surf.rect.width *= 2;
    end_surf.rect.height *= 2;

    UnloadImage(end_image);

    // start will run until `tint_amount` gets to 1.0f (tint_speed)
    timers["start"] = Timer(200.0f, false, true);
    timers["end"] = Timer(1.8f, false, false, end_evolution);
}

Evolution::~Evolution()
{
    UnloadTexture(*start_surf.texture);
    UnloadTexture(*start_mask.texture);
    UnloadTexture(*end_surf.texture);
}

void Evolution::Update(const double dt)
{
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }

    BeginTextureModeSafe(display_surface); // we don't want to clear background
    const RectangleU screen_rect = {0, 0, (float) GetScreenWidth(), (float) GetScreenHeight()};
    DrawRectangleRec(screen_rect.rectangle, Fade(BLACK, 200.0f / 255.0f));

    RectangleU position_rect = start_surf.rect;
    RectToCenter(position_rect, GetRectCenter(screen_rect));

    if (timers["start"].active)
    {
        if (tint_amount < 1.0f)
        {
            DrawTextureRec(
                    *start_surf.texture, start_surf.rect.rectangle, position_rect.pos, WHITE);

            tint_amount += tint_speed * dt;
            if (tint_amount >= 1.0f)
            {
                tint_amount = 1.0f;
            }
            DrawTextureRec(
                    *start_mask.texture, start_mask.rect.rectangle, position_rect.pos,
                    Fade(WHITE, tint_amount));
        }
        else
        {
            // force start deactivation
            timers["start"].Deactivate();
            if (!timers["end"].active)
            {
                timers["end"].Activate();
            }
        }
    }
    else if (timers["end"].active)
    {
        DrawTextureRec(*end_surf.texture, end_surf.rect.rectangle, position_rect.pos, WHITE);
    }
    EndTextureModeSafe();
}

bool Evolution::IsActive()
{
    return timers["start"].active || timers["end"].active;
}
