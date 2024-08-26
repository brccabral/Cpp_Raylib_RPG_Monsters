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

    // screen tint
    tint_surf = std::make_shared<rg::Surface>(
            display_surface->GetRect().width, display_surface->GetRect().height);
    tint_surf->SetAlpha(200);

    // white tint
    start_monster_surf_white = rg::mask::FromSurface(start_monster_surf).ToFrames(2, 4);
    start_monster_surf_white->SetColorKey(rl::BLACK);
    start_monster_surf_white->SetAlpha(tint_amount);
}

void Evolution::Update(const double dt)
{
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }
    if (!timers["start"].active)
    {
        display_surface->Blit(tint_surf, rg::math::Vector2{});
        if (tint_amount < 255)
        {
            const auto rect = start_monster_surf->GetRect().center(
                    {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
            display_surface->Blit(start_monster_surf, rect);

            tint_amount += tint_speed * dt;
            start_monster_surf_white->SetAlpha(tint_amount);
            display_surface->Blit(start_monster_surf_white, rect);
        }
        else
        {
            const auto rect =
                    end_monster_surf->GetRect().center({WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
            display_surface->Blit(end_monster_surf, rect);

            if (!timers["end"].active)
            {
                timers["end"].Activate();
            }
        }
    }
}
