#include "map.hpp"
#include "../game.hpp"
#include <fstream>
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

  int srcX, srcY;

  for (int y = 0; y < sizeY; y++) {
    for (int x = 0; x < sizeX; x++) {
      mapFile.get(tile);
      srcY = atoi(&tile) * mapTileSize;

      mapFile.get(tile);
      srcX = atoi(&tile) * mapTileSize;

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
