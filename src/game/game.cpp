#include "../game/game.hpp"
#include "../game/collision/collision.hpp"
#include "../game/components/colliderComponent/collider_component.hpp"
#include "../game/components/components.hpp"
#include "../game/map/map.hpp"
#include "../game/vector2d/vector_2d.hpp"
#include "../utility/utility.hpp"
#include <unistd.h>

Manager manager;
Map *map; // The map object

SDL_Renderer *Game::renderer = nullptr; // The renderer of the game
SDL_Event Game::event;                  // The event of the game
SDL_Rect Game::camera = {0, 0, 800, 640}; // The camera of the game

auto &tiles(manager.getGroup(Game::groupMap));
auto &players(manager.getGroup(Game::groupPlayers));
auto &colliders(manager.getGroup(Game::groupColliders));

auto &player(manager.addEntity());
auto &follower(manager.addEntity());

bool Game::isRunning = false; // Whether the game is running
bool Game::showColliders = false; // Whether to show colliders

// Constructor and Destructor
Game::Game() {}
Game::~Game() {}

/**
 * Initialize the game
 * @param title The title of the window
 * @param xpos The x position of the window
 * @param ypos The y position of the window
 * @param width The width of the window
 * @param height The height of the window
 * @param fullscreen Whether the window should be fullscreen
 */
void Game::init(const char *title, int xpos, int ypos, int width, int height,
                bool fullscreen) {

  // Set the flags for the window
  // This is because SDL_CreateWindow does not take a bool for fullscreen
  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  // Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Utility::Log("SDL_Init failed");
    isRunning = false;
    return;
  }

  // Create the window
  window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

  // Check if the window was created
  if (!window) {
    Utility::Log("Failed to create window");
    isRunning = false;
    return;
  }

  // Create the renderer
  renderer = SDL_CreateRenderer(window, -1, 0);

  // Check if the renderer was created
  if (!renderer) {
    Utility::Log("Failed to create renderer");
    isRunning = false;
    return;
  }

  // Set the renderer draw color to white
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // Set the game to running
  isRunning = true;

  // Create map if needed (currently not used)
  // map = std::make_unique<Map>();

  //int player_position_x = 32;
  //int player_position_y = 32;
  int player_scale = 1;
  bool is_animated = true;

  int mapSizeX = 16;
  int mapSizeY = 16;

  int map_scale = 2; // Scale of the map
  int map_tile_size = 32; // Size of the tiles in the map

  map = new Map("assets/maps/lvl1-tiles.png", map_scale, map_tile_size);
  map->LoadMap("assets/maps/lvl1.map", mapSizeX, mapSizeY);

  player.addComponent<TransformComponent>(player_scale);
  player.addComponent<SpriteComponent>("assets/player_animated.png", is_animated);
  player.addComponent<KeyboardController>();
  player.addComponent<ColliderComponent>("player");

  follower.addComponent<TransformComponent>(player_scale);
  follower.addComponent<SpriteComponent>("assets/follower.png", is_animated);
  // Make follower replicate player's movement with a delay (e.g., 20 frames)
  follower.addComponent<FollowDelayComponent>(&player, 10);

  // Spawn follower with an offset relative to player
  {
    auto &playerTr = player.getComponent<TransformComponent>();
    auto &followerTr = follower.getComponent<TransformComponent>();
    const float offsetX = -48.0f; // left of player
    const float offsetY = 0.0f;   // same vertical level
    followerTr.position.x = playerTr.position.x + offsetX;
    followerTr.position.y = playerTr.position.y + offsetY;
  }

  follower.addGroup(groupPlayers);
  player.addGroup(groupPlayers);
}

/**
 * Update the game
 */
void Game::update() {

  Vector2D playerPosition = player.getComponent<TransformComponent>().position;

  manager.refresh();
  manager.update();

  SDL_Rect playerCollider = player.getComponent<ColliderComponent>().collider;

  for( auto &collider : colliders) {
    SDL_Rect cCol = collider->getComponent<ColliderComponent>().collider;
    if(Collision::AABB(playerCollider, cCol)) {
      player.getComponent<TransformComponent>().position = playerPosition; // Reset player position if collision occurs
    }
  }

  int halfWidth = 400; // Half of the camera width
  int halfHeight = 320; // Half of the camera height

  camera.x = player.getComponent<TransformComponent>().position.x - halfWidth;
  camera.y = player.getComponent<TransformComponent>().position.y - halfHeight;

  if(camera.x < 0) {
    camera.x = 0; // Prevent camera from going out of bounds
  }

  if(camera.y < 0) {
    camera.y = 0; // Prevent camera from going out of bounds
  }
  if (camera.x > camera.w) {
    camera.x = camera.w; // Prevent camera from going out of bounds
  }
  if (camera.y > camera.h) {
    camera.y = camera.h; // Prevent camera from going out of bounds
  }
}

/**
 * Render the game
 */
void Game::render() {
  // Clear the renderer
  SDL_RenderClear(renderer);

  for (auto &tile : tiles) {
    tile->draw();
  }

  if(showColliders) {
    for (auto &collider : colliders) {
      collider->draw();
    }
  }

  for (auto &player : players) {
    player->draw();
  }

  // Present the renderer
  SDL_RenderPresent(renderer);
}

/**
 * Clean the game
 */
void Game::clean() {
  // Destroy the renderer and window
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  // Quit SDL
  SDL_Quit();

  Utility::Log("Game cleaned");
}

/**
 * Handle events
 */
void Game::handleEvents() {

  // Poll the event
  SDL_PollEvent(&event);

  // Check the type of the event
  switch (event.type) {
  case SDL_QUIT:
    isRunning = false;
    break;
  default:
    break;
  }
}