#pragma once
#include <rygame.hpp>
#include "settings.hpp"


class Evolution
{
public:

    Evolution(
            std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> &monster_frames,
            const std::string &start_monster, const std::string &end_monster,
            const std::shared_ptr<rg::font::Font> &font, const std::function<void()> &endEvolution,
            const std::vector<rg::Surface_Ptr> &star_animation_surfs);
    void Update(float dt);
    bool IsActive();

private:

    void DisplayStars(float dt);

    rg::Surface_Ptr display_surface = rg::display::GetSurface();
    rg::Frames_Ptr start_monster_surf = nullptr;
    rg::Frames_Ptr end_monster_surf = nullptr;

    std::map<std::string, rg::Timer> timers;

    rg::Surface_Ptr tint_surf = nullptr;
    rg::Frames_Ptr start_monster_surf_white = nullptr;
    float tint_amount = 0.0f;
    float tint_speed = 60.0f;

    rg::Surface_Ptr start_text_surf = nullptr;
    rg::Surface_Ptr end_text_surf = nullptr;

    std::vector<rg::Surface_Ptr> star_surfs;
    float frame_index = 0;
};
