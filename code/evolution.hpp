#pragma once
#include <rygame.hpp>
#include "settings.hpp"


class Evolution
{
public:

    Evolution() = default;
    Evolution(
            std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> &
            monster_frames,
            const std::string &start_monster, const std::string &end_monster,
            const rg::font::Font *font, const std::function<void()> &endEvolution,
            const std::vector<rg::Surface> &star_animation_surfs);
    void Update(float dt);
    bool IsActive();

private:

    void DisplayStars(float dt);

    rg::Surface *display_surface = &rg::display::GetSurface();
    rg::Frames start_monster_surf{};
    rg::Frames end_monster_surf{};

    std::unordered_map<std::string, rg::Timer> timers;

    rg::Surface tint_surf{};
    rg::Frames start_monster_surf_white{};
    float tint_amount = 0.0f;
    float tint_speed = 60.0f;

    rg::Surface start_text_surf{};
    rg::Surface end_text_surf{};

    std::vector<const rg::Surface *> star_surfs;
    float frame_index = 0;
};
