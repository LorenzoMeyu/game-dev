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

  void init() override;
  void update() override;

private:
  Entity *leaderEntity = nullptr;
  TransformComponent *leaderTransform = nullptr;
  TransformComponent *followerTransform = nullptr;
  SpriteComponent *followerSprite = nullptr;

  int delayFrames = 0;
  std::deque<Vector2D> positionHistory;
};

#endif
