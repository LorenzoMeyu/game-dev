// Ensure implementation of the class method declared in the header
#include "keyboard_controller.hpp"

void KeyboardController::init() {
  transform = &entity->getComponent<TransformComponent>();
  sprite = &entity->getComponent<SpriteComponent>();
}

void KeyboardController::update() {
  // Keyboard input handling
  if (Game::event.type == SDL_KEYDOWN) {
    sprite->play("walk");
    switch (Game::event.key.keysym.sym) {
    case SDLK_UP:
      transform->velocity.y = -1;
      break;
    case SDLK_DOWN:
      transform->velocity.y = 1;
      break;
    case SDLK_LEFT:
      transform->velocity.x = -1;
      break;
    case SDLK_RIGHT:
      transform->velocity.x = 1;
      break;
    default:
      break;
    }
  }

  if (Game::event.type == SDL_KEYUP) {
    switch (Game::event.key.keysym.sym) {
    case SDLK_UP:
      sprite->play("idle");
      transform->velocity.y = 0;
      break;
    case SDLK_DOWN:
      sprite->play("idle");
      transform->velocity.y = 0;
      break;
    case SDLK_LEFT:
      sprite->play("idle");
      transform->velocity.x = 0;
      break;
    case SDLK_RIGHT:
      sprite->play("idle");
      transform->velocity.x = 0;
      break;
    case SDLK_ESCAPE:
      Game::isRunning = false; // Stop the game when Escape is pressed
      break;
    case SDLK_F1:
      Game::showColliders = !Game::showColliders; // Toggle collider visibility
      break;
    default:
      break;
    }
  }
}