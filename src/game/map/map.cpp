#include "map.hpp"
#include "../game.hpp"
#include <fstream>
#include <cstdlib>
#include "../ECS/ECS.hpp"
#include "../components/tileComponent/tile_component.hpp"

extern Manager manager;

Map::Map(const char *mapFilePath, int mapScale, int mapTileSize) : mapFilePath(mapFilePath), mapScale(mapScale), mapTileSize(mapTileSize) {
  scaledSize = mapTileSize * mapScale;
}
Map::~Map() {}

/**
 * Load the map
 */
void Map::LoadMap(std::string path, int sizeX, int sizeY) {
  char tile;
  std::fstream mapFile;
  mapFile.open(path);
  if (!mapFile.is_open()) {
    Utility::Log("Failed to open map file: " + path);
    return;
  }

  int srcX, srcY;

  for (int y = 0; y < sizeY; y++) {
    for (int x = 0; x < sizeX; x++) {
      mapFile.get(tile);
      if (!std::isdigit(static_cast<unsigned char>(tile))) {
        Utility::Log("Invalid Y tile char in map file");
        return;
      }
      srcY = (tile - '0') * mapTileSize;

      mapFile.get(tile);
      if (!std::isdigit(static_cast<unsigned char>(tile))) {
        Utility::Log("Invalid X tile char in map file");
        return;
      }
      srcX = (tile - '0') * mapTileSize;

      AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
      mapFile.ignore();
    }
  }

  mapFile.ignore(); // Ignore the newline character after the last row

  for(int y = 0; y < sizeY; y++ ) {
    for(int x = 0; x < sizeX; x++) {
      mapFile.get(tile);
      if(tile == '1') {
        auto &collider(manager.addEntity());
        collider.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
        collider.addGroup(Game::groupColliders);
      }
      mapFile.ignore();
    }
  }

  mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) {
  int tile_size = 32;

  auto &tile(manager.addEntity());
  tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapTileSize, mapScale, mapFilePath);
  tile.addGroup(Game::groupMap);
}
