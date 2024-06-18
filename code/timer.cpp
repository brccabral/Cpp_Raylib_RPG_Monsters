#include "timer.h"

#include <raylib.h>

Timer::Timer(
        const float duration, const bool repeat, const bool autostart,
        const std::function<void()> &func)
    : duration(duration), repeat(repeat), autostart(autostart), func(func)
{
    if (autostart)
    {
        Activate();
    }
}

void Timer::Activate()
{
    active = true;
    start_time = GetTime();
}

void Timer::Deactivate()
{
    active = false;
    start_time = 0;
    if (repeat)
    {
        Activate();
    }
}

void Timer::Update()
{
    if (!active)
    {
        return;
    }
    const double currentTime = GetTime();
    if (currentTime - start_time >= duration)
    {
        if (func)
        {
            func();
        }
        Deactivate();
    }
}
