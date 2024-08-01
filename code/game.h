#pragma once
#include <rygame.h>


class Game
{
public:

    Game();
    ~Game();
    void run();

private:

    rg::Surface *display_surface = nullptr;
};
