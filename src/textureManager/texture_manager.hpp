#ifndef texture_manager_hpp
#define texture_manager_hpp

#include <SDL2/SDL.h>

/**
 * TextureManager class
 *
 * This class is used to load and draw textures
 *
 * @author: @iMeyu
 */
class TextureManager {
public:
  static SDL_Texture *LoadTexture(const char *texture);
  static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest);
  static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest,
                   SDL_RendererFlip flip);
};

#endif