#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "../../vector2d/vector_2d.hpp"
#include "../../ECS/ECS.hpp"
#include <cmath>

class TransformComponent : public Component {
public:
  Vector2D position;
  Vector2D velocity;

  int height = 32;
  int width = 32;
  int scale = 1;
  int speed = 4;

  TransformComponent();
  TransformComponent(float x, float y);
  TransformComponent(int scale);
  TransformComponent(float x, float y, int h, int w, int s);

  ~TransformComponent() override = default; // Default destructor
  
  void init() override;
  void update() override;

  float normalizeSpeed(float speed, float velocity);

  float getMagnitude() const {
    return std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
  }
};

#endif