#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "../../../textureManager/texture_manager.hpp"
#include "../../ECS/ECS.hpp"
#include "../transformComponent/transform_component.hpp"
#include "../animation.hpp"
#include "../../game.hpp"
#include <SDL2/SDL.h>
#include <map>
#include <string>

class SpriteComponent : public Component {
private:
  TransformComponent *transform;
  SDL_Texture *texture;
  SDL_Rect srcRect, destRect;

  bool animated = false;
  int frames = 0;
  int speed = 100;

public:
  int animationIndex = 0;
  std::map<std::string, Animation> animations;

  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

  SpriteComponent() = default;
  SpriteComponent(const char *path);
  SpriteComponent(const char *path, bool isAnimated);

  ~SpriteComponent();

  void setTexture(const char *path);

  void init() override;
  void update() override;
  void draw() override;

  void play(const char *animName);

};
#endif