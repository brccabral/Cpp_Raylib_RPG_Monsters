#include "evolution.hpp"


Evolution::Evolution(
        std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
                &monster_frames,
        const std::string &start_monster, const std::string &end_monster,
        const std::shared_ptr<rg::font::Font> &font, const std::function<void()> &endEvolution)
{
    auto start2x = rg::transform::Scale2x(monster_frames[start_monster][ANIMATIONSTATE_IDLE]);
    start_monster_surf = std::make_shared<rg::Frames>(start2x, 2, 4);
    auto end2x = rg::transform::Scale2x(monster_frames[end_monster][ANIMATIONSTATE_IDLE]);
    end_monster_surf = std::make_shared<rg::Frames>(end2x, 2, 4);

    // start will run until `tint_amount` gets to 1.0f (tint_speed)
    timers["start"] = rg::Timer(0.8f, false, true);
    timers["end"] = rg::Timer(1.8f, false, false, endEvolution);

    tint_surf = std::make_shared<rg::Surface>(
            display_surface->GetRect().width, display_surface->GetRect().height);
    tint_surf->SetAlpha(200);
}

void Evolution::Update(double dt)
{
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }
    if (!timers["start"].active)
    {
        display_surface->Blit(tint_surf, rg::math::Vector2{});
        const auto rect =
                start_monster_surf->GetRect().center({WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
        display_surface->Blit(start_monster_surf, rect);
    }
}
