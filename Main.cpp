#include "Game.hpp"

const int WIDTH = 960;
const int HEIGHT = 960;
#define TITLE "Classic_Chess_Game"

int main()
{
    Game *game = new Game;

    if (game->init(TITLE, WIDTH, HEIGHT))
    {
        game->~Game();
        delete game;
        return -1;
    }

    while (game->isRunning())
    {
        game->handleEvents();
        game->update();
        game->render();
        game->rest_for_a_little();
    }
    delete game;
    return 0;
}
