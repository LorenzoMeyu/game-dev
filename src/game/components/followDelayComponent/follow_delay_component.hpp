#ifndef FOLLOW_DELAY_COMPONENT_HPP
#define FOLLOW_DELAY_COMPONENT_HPP

#include <deque>
#include <cmath>

#include "../../ECS/ECS.hpp"
#include "../transformComponent/transform_component.hpp"
#include "../spriteComponent/sprite_component.hpp"

class FollowDelayComponent : public Component {
public:
  explicit FollowDelayComponent(Entity *leaderEntity, int delayFrames)
      : leaderEntity(leaderEntity), delayFrames(delayFrames) {}

  void init() override {
    followerTransform = &entity->getComponent<TransformComponent>();
    if (entity->hasComponent<SpriteComponent>()) {
      followerSprite = &entity->getComponent<SpriteComponent>();
    }
    leaderTransform = &leaderEntity->getComponent<TransformComponent>();
    if (delayFrames < 0) {
      delayFrames = 0;
    }
  }

  void update() override {
    // Lazily capture initial offset after external spawn offset is applied
    if (!hasInitialOffset) {
      initialOffset.x = followerTransform->position.x - leaderTransform->position.x;
      initialOffset.y = followerTransform->position.y - leaderTransform->position.y;
      hasInitialOffset = true;
    }

    // Lazily bind sprite if added after this component
    if (followerSprite == nullptr && entity->hasComponent<SpriteComponent>()) {
      followerSprite = &entity->getComponent<SpriteComponent>();
    }

    // Record leader position each frame
    positionHistory.emplace_back(leaderTransform->position);

    if (static_cast<int>(positionHistory.size()) > delayFrames) {
      const Vector2D previousPosition = followerTransform->position;
      const Vector2D delayedPosition = positionHistory.front();
      positionHistory.pop_front();

      // Move follower to delayed leader position plus the initial offset
      const float nextX = delayedPosition.x + initialOffset.x;
      const float nextY = delayedPosition.y + initialOffset.y;
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

private:
  Entity *leaderEntity = nullptr;
  TransformComponent *leaderTransform = nullptr;
  TransformComponent *followerTransform = nullptr;
  SpriteComponent *followerSprite = nullptr;

  int delayFrames = 0;
  std::deque<Vector2D> positionHistory;
  Vector2D initialOffset{};
  bool hasInitialOffset = false;
};

#endif
