// Ensure implementation of the class method declared in the header
#include "keyboard_controller.hpp"

void KeyboardController::init() {
  transform = &entity->getComponent<TransformComponent>();
  sprite = &entity->getComponent<SpriteComponent>();
  transform->speed = 2; // centralizza la speed qui
}

void KeyboardController::update() {
  const Uint8 *state = SDL_GetKeyboardState(nullptr);

  int dirX = (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) -
             (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]);
  int dirY = (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) -
             (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]);

  transform->velocity.x = dirX * transform->speed;
  transform->velocity.y = dirY * transform->speed;

  if (dirX != 0 || dirY != 0) {
    sprite->play("walk");
  } else {
    sprite->play("idle");
  }
}