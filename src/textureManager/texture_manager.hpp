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
  static SDL_Texture *LoadTexture(const char *texture, int width, int height);
};

#endif