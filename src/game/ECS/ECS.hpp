// ECS.hpp
// Entity Component System
// This file contains the ECS system for the game.
// It is used to manage the entities and components of the game.
// It is a simple and efficient system for managing the entities and components
// of the game.
#ifndef ECS_HPP
#define ECS_HPP

#include "../../utility/utility.hpp"
#include <algorithm> // Standard C++ algorithms (e.g., std::find)
#include <array>     // Fixed-size arrays
#include <bitset>    // Bitset management (useful for flags)
#include <memory>    // Smart pointers (e.g., std::unique_ptr)
#include <vector>    // Dynamic arrays (vectors)

// Forward declaration: we tell the compiler these classes exist
class Component;
class Entity;
class Manager;

using Group = std::size_t;

// Alias to represent the component ID as an unsigned integer
using ComponentID =
    std::size_t; // std::size_t is an integer type used for counting objects

/**
 * Function that generates a new unique ID for each component type.
 * Uses a static variable that is incremented at each call.
 * Returns an integer representing the component type ID.
 */
inline ComponentID getNewComponentTypeID() {
  static ComponentID lastID =
      0u;          // Static variable: keeps its value between calls
  return lastID++; // Returns the current value and then increments it
}

/**
 * Template function that returns a unique ID for each component type T.
 * The ID is generated only the first time the function is called for a certain
 * type T. noexcept means the function does not throw exceptions.
 */
template <typename T> inline ComponentID getComponentTypeID() noexcept {
  static ComponentID typeID =
      getNewComponentTypeID(); // Each type T will have its own static typeID
  return typeID;
}

// Maximum number of component types we can handle
constexpr std::size_t maxComponents = 32;

constexpr std::size_t maxGroups = 32;

// Bitset to keep track of which components an entity has (1 = present, 0 =
// absent)
using ComponentBitset = std::bitset<maxComponents>;

using GroupBitset = std::bitset<maxGroups>;

// Array of pointers to Component, one for each possible type
using ComponentArray = std::array<Component *, maxComponents>;

/**
 * Base class for all components.
 * Each component can be initialized, updated, and drawn.
 * The class is meant to be inherited.
 */
class Component {
public:
  Entity *entity; // Pointer to the entity the component belongs to

  // Virtual methods: can be overridden in derived classes
  virtual void init() {}   // Component initialization
  virtual void update() {} // Component update logic
  virtual void draw() {}   // Component drawing

  virtual ~Component() {} // Virtual destructor for correct deletion
};

/**
 * Class representing an entity in the ECS system.
 * An entity is composed of multiple components.
 */
class Entity {
private:
  Manager &manager;
  bool active = true; // Indicates whether the entity is active

  // Vector of unique smart pointers to Component: manages component memory
  std::vector<std::unique_ptr<Component>> components;

  // Array and bitset for fast access to components and checking their presence
  ComponentArray componentArray;
  ComponentBitset componentBitset;

  // Bitset for grouping entities
  GroupBitset groupBitset;

public:
  Entity(Manager &mManager) : manager(mManager) {}

  /**
   * Updates all components of the entity.
   */
  void update() {
    for (auto &c : components) {
      c->update();
    }
  }

  void draw() {
    for (auto &c : components) {
      c->draw();
    }
  }

  bool isActive() const {
    return active;
  } // Returns whether the entity is active

  void destroy() { active = false; } // Deactivates the entity

  /**
   * Checks if the entity belongs to a specific group.
   * Returns true if the entity belongs to the group, false otherwise.
   */
  bool hasGroup(Group mGroup) { return groupBitset[mGroup]; }

  /**
   * Adds a group to the entity.
   * The group is added to the entity's groupBitset.
   */
  void addGroup(Group mGroup);

  /**
   * Removes a group from the entity.
   * The group is removed from the entity's groupBitset.
   */
  void delGroup(Group mGroup);

  /**
   * Checks if the entity has a specific component.
   * Returns true if the component is present, false otherwise.
   */
  template <typename T> bool hasComponent() const {
    return componentBitset[getComponentTypeID<T>()];
  }

  /**
   * Adds a component to the entity.
   * The component is created with the given arguments and added to the entity.
   * @tparam T The type of the component to add
   * @tparam TArgs The types of the arguments to pass to the component
   * constructor
   * @param mArgs The arguments to pass to the component constructor
   * @return A reference to the added component
   */
  template <typename T, typename... TArgs> T &addComponent(TArgs &&...mArgs) {
    T *c(new T(std::forward<TArgs>(mArgs)...)); // Create the component
    c->entity = this;                           // Set the entity pointer
    std::unique_ptr<Component> uPtr{
        c}; // Create a unique pointer to the component
    components.emplace_back(std::move(uPtr)); // Add the component to the entity

    componentArray[getComponentTypeID<T>()] =
        c; // Add the component to the array
    componentBitset[getComponentTypeID<T>()] =
        true; // Set the bit for the component

    c->init(); // Initialize the component

    return *c; // Return a reference to the added component
  }

  /**
   * Gets a component from the entity.
   * Returns a reference to the component.
   */
  template <typename T> T &getComponent() const {
    auto ptr(componentArray[getComponentTypeID<T>()]);
    return *static_cast<T *>(ptr);
  }
};

class Manager {
private:
  std::vector<std::unique_ptr<Entity>>
      entities; // Vector of unique pointers to Entity

  std::array<std::vector<Entity *>, maxGroups> groupedEntities;

public:
  void update() {
    for (auto &e : entities)
      e->update();
  }

  void draw() {
    for (auto &e : entities)
      e->draw();
  }

  /**
   * Removes inactive entities from the entities vector.
   * This function is used to clean up inactive entities.
   */
  void refresh() {

    /**
     * Remove inactive entities from each group.
     */
    for (auto i(0u); i < maxGroups; i++) {
      auto &v(groupedEntities[i]);
      v.erase(std::remove_if(std::begin(v), std::end(v),
                             [i](Entity *mEntity) {
                               return !mEntity->isActive() ||
                                      !mEntity->hasGroup(i);
                             }),
              std::end(v));
    }

    entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                                  [](const std::unique_ptr<Entity> &mEntity) {
                                    return !mEntity->isActive();
                                  }),
                   std::end(entities));
  }

  /**
   * Adds an entity to a group.
   * The entity is added to the group's vector and the group is added to the
   * entity's groupBitset.
   */
  void addToGroup(Entity *mEntity, Group mGroup) {
    groupedEntities[mGroup].emplace_back(mEntity);
    // mEntity->addGroup(mGroup);
  }

  std::vector<Entity *> &getGroup(Group mGroup) {
    return groupedEntities[mGroup];
  }

  /**
   * Adds an entity to the entities vector.
   * The entity is created and added to the entities vector.
   * @return A reference to the added entity
   */
  Entity &addEntity() {
    Entity *e = new Entity(*this);
    std::unique_ptr<Entity> uPtr{e}; // Create a unique pointer to the entity
    entities.emplace_back(
        std::move(uPtr)); // Add the entity to the entities vector

    return *e; // Return a reference to the added entity
  }
};

#endif