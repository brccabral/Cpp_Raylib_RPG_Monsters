#include "game.h"
#include "settings.h"

int main()
{
    RectangleU rectangle_u = {2, 3, 4, 5};
    RectangleU new_rectangle_u = rectangle_u;
    RectangleU rectangle_u_move = std::move(new_rectangle_u);
    printf("%f", rectangle_u_move.height);

    Game game(WINDOW_WIDTH, WINDOW_HEIGHT);
    game.run();

    return 0;
}
