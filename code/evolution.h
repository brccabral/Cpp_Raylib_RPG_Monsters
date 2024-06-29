#pragma once
#include <map>
#include <string>
#include "sprite.h"
#include "support.h"
#include "tiledfont.h"


class Evolution
{
public:

    Evolution(
            std::map<std::string, Texture2D> *textures,
            std::map<std::string, animation_rects> *animation_frames,
            const std::string &start_monster, const std::string &end_monster, const Font &font,
            const std::function<void()> &end_evolution);
    ~Evolution();
    void Update(double dt);
    bool IsActive();

private:

    void EndEvolution();

    Font font;

    TiledTexture start_surf{};
    TiledTexture end_surf{};
    TiledTexture start_mask{};
    float tint_amount = 0.f;
    float tint_speed = 0.4f;

    TiledFont start_text;
    TiledFont end_text;

    std::map<std::string, Timer> timers;
};
