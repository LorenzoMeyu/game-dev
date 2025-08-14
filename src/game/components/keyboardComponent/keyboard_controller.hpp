#ifndef KEYBOARD_CONTROLLER_HPP
#define KEYBOARD_CONTROLLER_HPP

#include "../../../game/game.hpp"
#include "../../ECS/ECS.hpp"
#include "../spriteComponent/sprite_component.hpp"
#include "../transformComponent/transform_component.hpp"
#include <cmath>

class KeyboardController : public Component
{
public:
  TransformComponent *transform;
  SpriteComponent *sprite;

  void init() override;
  void update() override;

private: 
  bool upPressed = false;
  bool downPressed = false;
  bool leftPressed = false;
  bool rightPressed = false;
};

#endif