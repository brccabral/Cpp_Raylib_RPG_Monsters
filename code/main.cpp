
#include "game.h"
int main()
{
    // game has to leave scope before Quit() in order to unload resources
    {
        Game game;
        game.run();
    }

    rg::Quit();

    return 0;
}
