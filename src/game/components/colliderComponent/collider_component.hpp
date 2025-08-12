#ifndef COLLIDER_COMPONENT_HPP
#define COLLIDER_COMPONENT_HPP

#include "../../game.hpp"
#include "../../ECS/ECS.hpp"
#include "../transformComponent/transform_component.hpp"
#include "../../../textureManager/texture_manager.hpp"
#include <SDL2/SDL.h>
#include <string>

class ColliderComponent : public Component
{
public:
  SDL_Rect collider;
  std::string tag;

  SDL_Texture *texture;
  SDL_Rect srcRect, destRect;

  TransformComponent *transform;

  ColliderComponent(std::string tag);
  ColliderComponent(std::string tag, int xPos, int yPos, int size);

  ~ColliderComponent() override; // Out-of-line destructor declaration

  void init() override;
  void update() override;
  void draw() override;
};

#endif