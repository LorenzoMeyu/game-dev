#include "sprite_component.hpp"

SpriteComponent::SpriteComponent(const char *path) {
    setTexture(path);
}

SpriteComponent::SpriteComponent(const char *path, bool isAnimated) {
    setTexture(path);
    animated = isAnimated;
    int idleIndex = 0;
    int walkIndex = 1;

    int framesNumber = 10;
    int reproductionSpeed = 100;

    Animation idle = Animation(idleIndex, framesNumber, reproductionSpeed);
    Animation walk = Animation(walkIndex, framesNumber, reproductionSpeed);

    animations.emplace("idle", idle);
    animations.emplace("walk", walk);

    play("idle");
}

SpriteComponent::~SpriteComponent() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void SpriteComponent::init() {
    transform = &entity->getComponent<TransformComponent>();

    srcRect.x = srcRect.y = 0;
    srcRect.w = transform->width;
    srcRect.h = transform->height;
}

void SpriteComponent::update() {
     if (animated && frames > 0) {
      srcRect.x =
          srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
    }

    srcRect.y = animationIndex * transform->height;

    destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
    destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
    destRect.w = transform->width * transform->scale;
    destRect.h = transform->height * transform->scale;
}

void SpriteComponent::setTexture(const char *path) {
  texture = TextureManager::LoadTexture(path);
}

void SpriteComponent::draw() {
    if (texture) {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }
}

void SpriteComponent::play(const char *animName) {
    const std::string key(animName);
    auto it = animations.find(key);
    if (it != animations.end()) {
      frames = it->second.frames;
      animationIndex = it->second.index;
      speed = it->second.speed;
    }
}