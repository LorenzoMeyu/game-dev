#ifndef MAP_HPP
#define MAP_HPP

#include <SDL2/SDL.h>
#include <string>

/**
 * Map class
 *
 * This class is used to create a map
 *
 * @author: @iMeyu
 */
class Map {
public:
  Map(const char *mapFilePath, int mapScale, int mapTileSize);
  ~Map();

  void AddTile(int srcX, int srcY, int xpos, int ypos);
  void LoadMap(std::string path, int sizeX, int sizeY);

private:
  const char *mapFilePath;
  int mapScale;
  int mapTileSize;
  int scaledSize;
};

#endif