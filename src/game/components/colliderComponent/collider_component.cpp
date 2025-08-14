#include "./collider_component.hpp"
#include "../../../textureManager/texture_manager.hpp"

ColliderComponent::ColliderComponent(std::string tag) { this->tag = tag; }

ColliderComponent::ColliderComponent(std::string tag, int xPos, int yPos,
                                     int size) {
  this->tag = tag;
  collider.x = xPos;
  collider.y = yPos;
  collider.w = size;
  collider.h = size;
}

ColliderComponent::ColliderComponent(std::string tag, int xPos, int yPos,
                                     int width, int height) {
  this->tag = tag;
  collider.x = xPos;
  collider.y = yPos;
  collider.w = width;
  collider.h = height;
}

ColliderComponent::ColliderComponent(std::string tag, int xPos, int yPos,
                                     int width, int height, int offsetX,
                                     int offsetY) {
  this->tag = tag;
  collider.x = xPos;
  collider.y = yPos;
  collider.w = width;
  collider.h = height;
  this->offsetX = offsetX;
  this->offsetY = offsetY;
}

void ColliderComponent::init() {
  if (!entity->hasComponent<TransformComponent>()) {
    entity->addComponent<TransformComponent>();
  }

  transform = &entity->getComponent<TransformComponent>();

  texture = TextureManager::LoadTexture("assets/col-sprite.png");
  srcRect = {0, 0, 32, 32};
  destRect = {collider.x, collider.y, collider.w, collider.h};

  // Game::colliders.push_back(this);
}

void ColliderComponent::update() {
  if (tag != "terrain") {
    collider.x = static_cast<int>(transform->position.x) + offsetX;
    collider.y = static_cast<int>(transform->position.y) + offsetY;
  }

  destRect.x = collider.x - Game::camera.x;
  destRect.y = collider.y - Game::camera.y;
}

void ColliderComponent::draw() {
  if (texture) {
    TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
  }
}

// Provide out-of-line virtual destructor to ensure vtable emission
ColliderComponent::~ColliderComponent() {
  if (texture) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
}