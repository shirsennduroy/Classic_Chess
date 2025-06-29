#include "King.hpp"
#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP // AS THIRD

class TextureManager
{
public:
    static SDL_Texture *LoadTexture(const std::string &fileName, SDL_Renderer *renderer);
};

#endif
