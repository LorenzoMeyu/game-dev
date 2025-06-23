#include "texture_manager.hpp"
#include "../game/game.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

/**
 * Load a texture from a file
 *
 * @param texture The path to the texture file
 * @param ren The renderer to load the texture into
 * @return The loaded texture
 */
SDL_Texture *TextureManager::LoadTexture(const char *texture, int width,
                                         int height) {
  // Load the texture from the file
  SDL_Surface *tempSurface = IMG_Load(texture);

  // Check if the surface was loaded
  if (!tempSurface) {
    std::cout << "Failed to load texture: " << texture << std::endl;
    std::cout << "SDL_image Error: " << IMG_GetError() << std::endl;
    return nullptr;
  }

  // Create a new surface with the desired dimensions
  SDL_Surface *scaledSurface = SDL_CreateRGBSurface(
      tempSurface->flags, width, height, tempSurface->format->BitsPerPixel,
      tempSurface->format->Rmask, tempSurface->format->Gmask,
      tempSurface->format->Bmask, tempSurface->format->Amask);

  if (!scaledSurface) {
    std::cout << "Failed to create scaled surface" << std::endl;
    SDL_FreeSurface(tempSurface);
    return nullptr;
  }

  // Scale the old surface onto the new surface
  SDL_BlitScaled(tempSurface, NULL, scaledSurface, NULL);
  SDL_FreeSurface(tempSurface); // Free the original surface

  // Create a texture from the surface
  SDL_Texture *tex =
      SDL_CreateTextureFromSurface(Game::renderer, scaledSurface);

  // Check if the texture was created
  if (!tex) {
    std::cout << "Failed to create texture" << std::endl;
    SDL_FreeSurface(scaledSurface);
    return nullptr;
  }

  // Free the surface
  SDL_FreeSurface(scaledSurface);

  std::cout << "Texture loaded" << std::endl;

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