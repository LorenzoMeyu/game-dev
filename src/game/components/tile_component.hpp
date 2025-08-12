#ifndef TILE_COMPONENT_HPP
#define TILE_COMPONENT_HPP

#include "../ECS/ECS.hpp"
#include "./spriteComponent/sprite_component.hpp"
#include "./transformComponent/transform_component.hpp"
#include <SDL2/SDL.h>
#include "./colliderComponent/collider_component.hpp"

class TileComponent : public Component {
public:

  SDL_Texture *texture; 
  SDL_Rect srcRect, destRect;
  Vector2D position;

  TileComponent() = default;
  TileComponent(int srcX, int srcY, int xpos, int ypos, int tile_size, int tile_scale, const char *path) {
    texture = TextureManager::LoadTexture(path);

    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = srcRect.h = tile_size;

    position.x = xpos;
    position.y = ypos;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = destRect.h = tile_size * tile_scale;
  }

  void update() override {
    destRect.x = position.x - Game::camera.x;
    destRect.y = position.y - Game::camera.y;
  }

  void draw() override {
    TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
  }

  ~TileComponent() {
    SDL_DestroyTexture(texture);
  }

};

#endif