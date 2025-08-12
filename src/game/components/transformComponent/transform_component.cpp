#include "transform_component.hpp"

TransformComponent::TransformComponent() {
    position.Zero();
}

TransformComponent::TransformComponent(float x, float y) {
    position.x = x;
    position.y = y;
}

TransformComponent::TransformComponent(int scale) {
    //position.Zero();
    position.x = 400;
    position.y = 320;
    this->scale = scale;
}

TransformComponent::TransformComponent(float x, float y, int h, int w, int s) {
    position.x = x;
    position.y = y;
    height = h;
    width = w;
    scale = s;
}

void TransformComponent::init() {
    velocity.Zero();
}

void TransformComponent::update() {
    position.x += TransformComponent::normalizeSpeed(speed, velocity.x);
    position.y += TransformComponent::normalizeSpeed(speed, velocity.y);
}

float TransformComponent::normalizeSpeed(float speed, float velocity) {
    float normalizedSpeed = 0.0f;
    float normalizer = 0.0f;
    float magnitude = 0.0f;

    magnitude = TransformComponent::getMagnitude();
    speed *= velocity;
    
    // If the magnitude is zero, return the speed as is to avoid division by zero
    if (magnitude == 0) {
        return speed;
    }

    normalizer = std::abs(velocity / magnitude);

    normalizedSpeed = speed * normalizer;
    return normalizedSpeed;
}