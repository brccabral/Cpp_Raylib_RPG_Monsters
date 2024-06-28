#pragma once
#include <functional>


class Timer
{
public:

    // Default constructor
    Timer() : duration(0.0f), repeat(false), autostart(false), func([]() {})
    {}

    // Parameterized constructor
    explicit
    Timer(float duration, bool repeat = false, bool autostart = false,
          const std::function<void()> &func = std::function<void()>{});
    ~Timer();
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
