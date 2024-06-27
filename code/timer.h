#pragma once
#include <functional>


class Timer
{
public:

    explicit Timer(
            float duration, bool repeat = false, bool autostart = false,
            const std::function<void()> &func = []() {});
    void Activate();
    void Deactivate();
    void Update();
    bool active{};
    float duration;

private:

    bool repeat;
    bool autostart;
    std::function<void()> func;
    double start_time{};
};
