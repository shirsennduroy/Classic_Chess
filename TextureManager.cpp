#include "TextureManager.hpp"

SDL_Texture *TextureManager::LoadTexture(const std::string &fileName, SDL_Renderer *rend) // called by ----->  void Board::loadPieceTextures()
{
    SDL_Surface *tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface)
    {
        std::cerr << "Failed to load image: " << fileName << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture *texure = SDL_CreateTextureFromSurface(rend, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texure;
}
