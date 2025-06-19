#ifndef game_object_hpp
#define game_object_hpp

#include <SDL2/SDL.h>

/**
 * GameObject class
 *
 * This class is used to create a game object
 *
 * @author: @iMeyu
 */
class GameObject {
public:
  GameObject(const char *textureSheet, int x, int y, int width, int height);
  ~GameObject();

  void Update();
  void Render();

private:
  int xpos, ypos;             // The position of the object
  SDL_Texture *objTexture;    // The texture of the object
  SDL_Rect srcRect, destRect; // The source and destination rectangles
};

#endif