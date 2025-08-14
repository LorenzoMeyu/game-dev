#ifndef COLLIDER_COMPONENT_HPP
#define COLLIDER_COMPONENT_HPP

#include "../../ECS/ECS.hpp"
#include "../../game.hpp"
#include "../transformComponent/transform_component.hpp"
#include <SDL2/SDL.h>
#include <string>

class ColliderComponent : public Component {
public:
  SDL_Rect collider;
  std::string tag;

  SDL_Texture *texture;
  SDL_Rect srcRect, destRect;

  TransformComponent *transform;

  // Optional local offset from the entity transform position
  int offsetX = 0;
  int offsetY = 0;

  ColliderComponent(std::string tag);
  ColliderComponent(std::string tag, int xPos, int yPos, int size);
  ColliderComponent(std::string tag, int xPos, int yPos, int width, int height);
  ColliderComponent(std::string tag, int xPos, int yPos, int width, int height,
                    int offsetX, int offsetY);

  ~ColliderComponent() override; // Out-of-line destructor declaration

  void init() override;
  void update() override;
  void draw() override;
};

#endif