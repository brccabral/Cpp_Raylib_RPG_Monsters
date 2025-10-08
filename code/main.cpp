#include "game.hpp"

int main()
{
    rg::Init(rl::LOG_WARNING);
    {
        Game game;
        game.run();
    }
    rg::Quit();

    return 0;
}
