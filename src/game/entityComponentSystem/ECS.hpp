#ifndef ECS_HPP
#define ECS_HPP

#include <algorithm> // For algorithm
#include <array>     // For array
#include <bitset>    // For bitset
#include <iostream>  // For input/output
#include <memory>    // For memory
#include <vector>    // For vector

class Component;
class Entity;

using ComponentID = std::size_t; // The ID of the component

/**
 * Get the component type ID
 * This function is used to get the component type ID
 *
 * @return The component type ID
 */
inline ComponentID getComponentID() {
  static ComponentID lastID = 0;
  return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
  static ComponentID typeID = getComponentID();
  return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

class Component {
public:
  Entity *entity;

  virtual void init() {}
  virtual void update() {};
  virtual void draw() {};

  virtual ~Component() {}
};

class Entity {
public:
  bool active = true;
  std::vector<std::unique_ptr<Component>> components;

  ComponentArray componentArray;
  ComponentBitset componentBitset;
};
#endif