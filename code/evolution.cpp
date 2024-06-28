#include "evolution.h"

Evolution::Evolution(
        std::map<std::string, Texture2D> *textures,
        std::map<std::string, animation_rects> *animation_frames, const std::string &start_monster,
        const std::string &end_monster, Font font, const std::function<void()> &end_evolution)
{
    Image start_image = LoadImageFromTexture((*textures)[start_monster]);
    ImageResize(&start_image, start_image.width * 2, start_image.height * 2);
    start_texture = LoadTextureFromImage(start_image);
    UnloadImage(start_image);

    start_animation_rect = (*animation_frames)[start_monster][ANIMATION_IDLE][0];
    start_animation_rect.width *= 2;
    start_animation_rect.height *= 2;

    Image end_image = LoadImageFromTexture((*textures)[end_monster]);
    ImageResize(&end_image, end_image.width * 2, end_image.height * 2);
    end_texture = LoadTextureFromImage(end_image);
    UnloadImage(end_image);

    end_animation_rect = (*animation_frames)[end_monster][ANIMATION_IDLE][0];
    end_animation_rect.width *= 2;
    end_animation_rect.height *= 2;

    timers["start"] = Timer(0.8f, false, true);
    timers["end"] = Timer(1.8f, false, false, end_evolution);
}

void Evolution::Update(double dt)
{
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }
    if (!timers["start"].active)
    {
        BeginTextureModeSafe(display_surface);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 200.0f / 255.0f));
        EndTextureModeSafe();
    }
}
