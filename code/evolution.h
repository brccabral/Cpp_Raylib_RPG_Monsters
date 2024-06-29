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
            const char *start_monster_name, const char *end_monster_name, const Font &font,
            const std::function<void()> &end_evolution,
            const std::vector<Texture2D> &star_animation_textures, int level, int index);
    ~Evolution();
    void Update(double dt);
    bool IsActive();

    const char *end_monster;
    int level;
    int index;

private:

    void EndEvolution();
    void DisplayStars(double dt);

    Font font;

    TiledTexture start_surf{};
    TiledTexture end_surf{};
    TiledTexture start_mask{};
    float tint_amount = 0.f;
    float tint_speed = 0.4f;

    const char *start_monster;
    TiledFont *start_font = nullptr;
    TiledFont *end_font = nullptr;

    std::map<std::string, Timer> timers;

    std::vector<Texture2D> star_textures;
    Vector2 star_pos{};
    float frame_index{};
};
