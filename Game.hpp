#include "Board.hpp" //second
#ifndef GAME_HPP
#define GAME_HPP // AS FIRST

class Game
{
public:
    Game();
    ~Game();
    bool init(const char *title, int width, int height);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning() const;
    void rest_for_a_little();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool running;
    int x, y;

    Board *board; // new second
};

#endif
