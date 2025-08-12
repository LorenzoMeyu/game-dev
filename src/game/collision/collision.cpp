#include "collision.hpp"
#include "../components/colliderComponent/collider_component.hpp"

bool Collision::AABB(const SDL_Rect &rectA, const SDL_Rect &rectB) {
  return SDL_HasIntersection(&rectA, &rectB);
}

bool Collision::AABB(const ColliderComponent &colA,
                     const ColliderComponent &colB) {
  if (SDL_HasIntersection(&colA.collider, &colB.collider) && &colA.tag != &colB.tag) {
    return true;
  }
  return false;
}