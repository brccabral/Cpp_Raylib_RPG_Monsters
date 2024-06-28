#pragma once
#include <map>
#include <string>
#include "sprite.h"
#include "support.h"


class Evolution
{
public:

    Evolution(
            std::map<std::string, Texture2D> *textures,
            std::map<std::string, animation_rects> *animation_frames,
            const std::string &start_monster, const std::string &end_monster, Font font,
            const std::function<void()> &end_evolution);
    ~Evolution();
    void Update(double dt);

private:

    void EndEvolution();

    TiledTexture start_surf;
    TiledTexture end_surf;

    TiledTexture start_mask;

    std::map<std::string, Timer> timers;
};
