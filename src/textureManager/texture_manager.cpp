#include "texture_manager.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

/**
 * Load a texture from a file
 *
 * @param texture The path to the texture file
 * @param ren The renderer to load the texture into
 * @return The loaded texture
 */
SDL_Texture *TextureManager::LoadTexture(const char *texture, SDL_Renderer *ren,
                                         int width, int height) {
  // Load the texture from the file
  SDL_Surface *tempSurface = IMG_Load(texture);

  // Check if the surface was loaded
  if (!tempSurface) {
    std::cout << "Failed to load texture" << std::endl;
    return nullptr;
  }

  // Set the width and height of the surface
  tempSurface->w = width;
  tempSurface->h = height;

  // Create a texture from the surface
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tempSurface);

  // Set the width and height of the texture
  SDL_QueryTexture(tex, NULL, NULL, &width, &height);

  // Check if the texture was created
  if (!tex) {
    std::cout << "Failed to create texture" << std::endl;
    return nullptr;
  }

  // Free the surface
  SDL_FreeSurface(tempSurface);

  std::cout << "Texture loaded" << std::endl;

  // Return the texture
  return tex;
}