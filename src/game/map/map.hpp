#include <SDL2/SDL.h>

#ifndef MAP_HPP
#define MAP_HPP

/**
 * Map class
 *
 * This class is used to create a map
 *
 * @author: @iMeyu
 */
class Map {
public:
  Map();
  ~Map();

  void LoadMap();
  void DrawMap();

private:
  SDL_Rect src, dest;

  SDL_Texture *dirt;
  SDL_Texture *grass;
  SDL_Texture *water;

  int map[20][25];
};

#endif