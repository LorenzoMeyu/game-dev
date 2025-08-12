#include "follow_delay_component.hpp"

void FollowDelayComponent::init() {
    followerTransform = &entity->getComponent<TransformComponent>();
    if (entity->hasComponent<SpriteComponent>()) {
      followerSprite = &entity->getComponent<SpriteComponent>();
    }
    leaderTransform = &leaderEntity->getComponent<TransformComponent>();
    if (delayFrames < 0) {
      delayFrames = 0;
    }
}

void FollowDelayComponent::update() {
    // Lazily bind sprite if added after this component
    if (followerSprite == nullptr && entity->hasComponent<SpriteComponent>()) {
      followerSprite = &entity->getComponent<SpriteComponent>();
    }

    // Record leader position each frame
    positionHistory.emplace_back(leaderTransform->position);

    if (static_cast<int>(positionHistory.size()) > (delayFrames)) {
      const Vector2D previousPosition = followerTransform->position;
      const Vector2D delayedPosition = positionHistory.front();
      positionHistory.pop_front();

      // Move follower to delayed leader position plus the initial offset
      const float nextX = delayedPosition.x;
      const float nextY = delayedPosition.y; 
      followerTransform->position.x = nextX;
      followerTransform->position.y = nextY;

      // Optional: update animation based on movement
      if (followerSprite != nullptr) {
        const bool moved = (previousPosition.x != nextX) ||
                           (previousPosition.y != nextY);
        followerSprite->play(moved ? "walk" : "idle");
      }
    }
}