#include "evolution.hpp"


Evolution::Evolution(
        std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> &
        monster_frames,
        const std::string &start_monster, const std::string &end_monster,
        const rg::font::Font *font, const std::function<void()> &endEvolution,
        const std::vector<rg::Surface> *star_animation_surfs)
    : star_surfs(star_animation_surfs)
{
    auto start2x = rg::transform::Scale2x(&monster_frames[start_monster][ANIMATIONSTATE_IDLE]);
    start_monster_surf = rg::Frames(&start2x, 2, 4);
    auto end2x = rg::transform::Scale2x(&monster_frames[end_monster][ANIMATIONSTATE_IDLE]);
    end_monster_surf = rg::Frames(&end2x, 2, 4);

    // start will run until `tint_amount` gets to 1.0f (tint_speed)
    timers["start"] = rg::Timer(200.0f, false, false);
    timers["end"] = rg::Timer(1.8f, false, false, endEvolution);

    // screen tint
    tint_surf = rg::Surface(
            (int) display_surface->GetRect().width, (int) display_surface->GetRect().height);
    tint_surf.SetAlpha(200);

    // white tint
    start_monster_surf_white = rg::mask::FromSurface(&start_monster_surf).ToFrames(2, 4);
    start_monster_surf_white.SetColorKey(rl::BLACK);
    start_monster_surf_white.SetAlpha(tint_amount);

    // text
    start_text_surf =
            font->render(
                    rl::TextFormat("%s is evolving", start_monster.c_str()),
                    Settings::GetInstance().COLORS["black"]);
    end_text_surf = font->render(
            rl::TextFormat("%s evolved into %s", start_monster.c_str(), end_monster.c_str()),
            Settings::GetInstance().COLORS["black"]);
}

void Evolution::Update(const float dt)
{
    display_surface->Blit(&tint_surf, rg::math::Vector2{});
    if (timers["start"].active)
    {
        if (tint_amount < 255)
        {
            const auto rect = start_monster_surf.GetRect().center(
                    {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
            display_surface->Blit(&start_monster_surf, rect);

            tint_amount += tint_speed * dt;
            start_monster_surf_white.SetAlpha(tint_amount);
            display_surface->Blit(&start_monster_surf_white, rect);

            const auto text_rect =
                    start_text_surf.GetRect().midtop(rect.midbottom() + rg::math::Vector2{0, 20});
            rg::draw::rect(
                    display_surface, Settings::GetInstance().COLORS["white"],
                    text_rect.inflate(20, 20), 0, 5);
            display_surface->Blit(&start_text_surf, text_rect);
        }
        else
        {
            // force start deactivation
            timers["start"].Deactivate();
            if (!timers["end"].active)
            {
                timers["end"].Activate();
            }
        }
    }
    else if (timers["end"].active)
    {
        const auto rect =
                end_monster_surf.GetRect().center({WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
        display_surface->Blit(&end_monster_surf, rect);

        const auto text_rect =
                end_text_surf.GetRect().midtop(rect.midbottom() + rg::math::Vector2{0, 20});
        rg::draw::rect(
                display_surface, Settings::GetInstance().COLORS["white"], text_rect.inflate(20, 20),
                0, 5);
        display_surface->Blit(&end_text_surf, text_rect);
        DisplayStars(dt);
    }
    // update timers after display because "end" destroys ~Evolution() and
    // we loose all pointers
    for (auto &timer: timers | std::views::values)
    {
        timer.Update();
    }
}

bool Evolution::IsActive()
{
    return timers["start"].active || timers["end"].active;
}

void Evolution::Activate()
{
    timers["start"].Activate();
}

void Evolution::DisplayStars(const float dt)
{
    frame_index += 20 * dt;
    if (frame_index < (float) star_surfs->size())
    {
        auto &frame = (*star_surfs)[int(frame_index)];
        const auto rect = frame.GetRect().center({WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
        display_surface->Blit(&frame, rect);
    }
}
