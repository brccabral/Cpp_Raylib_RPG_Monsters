#pragma once
#include <rygame.hpp>
#include "settings.hpp"


class Evolution
{
public:

    Evolution(
            std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
                    &monster_frames,
            const std::string &start_monster, const std::string &end_monster,
            const std::shared_ptr<rg::font::Font> &font, const std::function<void()> &endEvolution,
            const std::vector<std::shared_ptr<rg::Surface>> &star_animation_surfs);
    void Update(double dt);
    bool IsActive();

private:

    void DisplayStars(float dt);

    std::shared_ptr<rg::Surface> display_surface = rg::display::GetSurface();
    std::shared_ptr<rg::Frames> start_monster_surf = nullptr;
    std::shared_ptr<rg::Frames> end_monster_surf = nullptr;

    std::map<std::string, rg::Timer> timers;

    std::shared_ptr<rg::Surface> tint_surf = nullptr;
    std::shared_ptr<rg::Frames> start_monster_surf_white = nullptr;
    float tint_amount = 0.0f;
    float tint_speed = 60.0f;

    std::shared_ptr<rg::Surface> start_text_surf = nullptr;
    std::shared_ptr<rg::Surface> end_text_surf = nullptr;

    std::vector<std::shared_ptr<rg::Surface>> star_surfs;
    float frame_index = 0;
};
