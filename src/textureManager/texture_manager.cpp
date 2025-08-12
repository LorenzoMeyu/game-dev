#include "texture_manager.hpp"
#include "../game/game.hpp"
#include "../utility/utility.hpp"
#include <SDL2/SDL_image.h>

/**
 * Load a texture from a file
 *
 * @param texture The path to the texture file
 * @param ren The renderer to load the texture into
 * @return The loaded texture
 */
SDL_Texture *TextureManager::LoadTexture(const char *texture) {
  // Load the texture from the file
  SDL_Surface *tempSurface = IMG_Load(texture);

  // Check if the surface was loaded
  if (!tempSurface) {
    Utility::Log("Failed to load texture: " + std::string(texture));
    Utility::Log("SDL_image Error: " + std::string(IMG_GetError()));
    return nullptr;
  }

  // Create a texture from the surface
  SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
  // Free the surface now that we're done with it
  SDL_FreeSurface(tempSurface);

  // Check if the texture was created
  if (!tex) {
    Utility::Log("Failed to create texture from " + std::string(texture));
    return nullptr;
  }

  // Return the texture
  return tex;
}

/**
 * Draw a texture to the screen
 *
 * @param tex The texture to draw
 * @param src The source rectangle
 * @param dest The destination rectangle
 */
void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest) {
  SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest,
                          SDL_RendererFlip flip) {
  SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, 0, NULL, flip);
}