#include "Game.hpp"

Game::Game() : window(nullptr), renderer(nullptr), running(false) {}

bool Game::init(const char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return true;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "CreateWindow Error: " << SDL_GetError() << std::endl;
        return true;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "CreateRenderer Error: " << SDL_GetError() << std::endl;
        return true;
    }

    board = new Board(renderer);
    // Calling Board(SDL_REnderer *renderer); at Board.hpp and defination at Board.cpp
    //------------------------------------------

    running = true;
    return false;
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_SPACE || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;
        //------------------------         from this to___
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {

            SDL_GetMouseState(&x, &y);
            board->handleClick(x, y);
        }
        //-----------------------         this is New Fourth
        /*
        getting the mouse position at the window surface via SDL_GetMouseState(&x,&y)
        */
    }
}

void Game::update()
{
    // Will add game logic here
}

void Game::render()
{
    // SDL_SetRenderDrawColor(renderer, 0x0, 0xdf, 0xdf, 0xff);
    // SDL_RenderClear(renderer);

    board->draw(); // board draw the Grid . Calling draw where defination at Board.cpp
    //-----------------------------------------------------------------

    SDL_RenderPresent(renderer); // Showing the renderer
}

void Game::clean()
{
    if (board)
        delete board;

    if (renderer)
        SDL_DestroyRenderer(renderer);

    if (window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}

bool Game::isRunning() const
{
    return running;
}

void Game::rest_for_a_little()
{
    SDL_Delay(16); // ~60 FPS
}

Game::~Game()
{
    clean();
}