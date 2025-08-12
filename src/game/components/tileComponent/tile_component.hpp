#ifndef TILE_COMPONENT_HPP
#define TILE_COMPONENT_HPP

#include "../../ECS/ECS.hpp"
#include "../spriteComponent/sprite_component.hpp"
#include "../transformComponent/transform_component.hpp"
#include <SDL2/SDL.h>
#include "../colliderComponent/collider_component.hpp"

class TileComponent : public Component {
public:

  SDL_Texture *texture; 
  SDL_Rect srcRect, destRect;
  Vector2D position;

  TileComponent() = default;
  TileComponent(int srcX, int srcY, int xpos, int ypos, int tile_size, int tile_scale, const char *path);

  ~TileComponent();

  void update() override;
  void draw() override;

};

#endif