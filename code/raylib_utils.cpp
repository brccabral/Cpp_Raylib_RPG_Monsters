#include "raylib_utils.h"

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
