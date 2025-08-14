#include "../game/game.hpp"
#include "../game/collision/collision.hpp"
#include "../game/components/colliderComponent/collider_component.hpp"
#include "../game/components/keyboardComponent/keyboard_controller.hpp"
#include "../game/components/spriteComponent/sprite_component.hpp"
#include "../game/map/map.hpp"
#include "../game/vector2d/vector_2d.hpp"
#include "../utility/utility.hpp"
#include <memory>

Manager manager;
std::unique_ptr<Map> map; // The map object

SDL_Renderer *Game::renderer = nullptr;   // The renderer of the game
SDL_Event Game::event;                    // The event of the game
SDL_Rect Game::camera = {0, 0, 800, 640}; // The camera of the game

auto &tiles(manager.getGroup(Game::groupMap));
auto &players(manager.getGroup(Game::groupPlayers));
auto &colliders(manager.getGroup(Game::groupColliders));

auto &player(manager.addEntity());
auto &follower(manager.addEntity());
auto &follower2(manager.addEntity());

bool Game::isRunning = false;     // Whether the game is running
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

  // Initialize SDL_image for PNG support
  const int imgFlags = IMG_INIT_PNG;
  if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
    Utility::Log("IMG_Init failed: " + std::string(IMG_GetError()));
    // Not fatal for now; textures will fail to load and log accordingly
  }

  // Create the window
  window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

  // Check if the window was created
  if (!window) {
    Utility::Log("Failed to create window");
    isRunning = false;
    return;
  }

  // Create the renderer (accelerated + vsync)
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // Check if the renderer was created
  if (!renderer) {
    Utility::Log("Failed to create renderer");
    isRunning = false;
    return;
  }

  // Set the renderer draw color to black
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // Set the game to running
  isRunning = true;

  // Set camera size from window size
  camera = {0, 0, width, height};

  // int player_position_x = 32;
  // int player_position_y = 32;
  int player_scale = 1;
  bool is_animated = true;

  int mapSizeX = 16;
  int mapSizeY = 16;

  int map_scale = 2;      // Scale of the map
  int map_tile_size = 32; // Size of the tiles in the map

  map = std::make_unique<Map>("assets/maps/lvl1-tiles.png", map_scale,
                              map_tile_size);
  map->LoadMap("assets/maps/lvl1.map", mapSizeX, mapSizeY);

  player.addComponent<TransformComponent>(player_scale);
  player.addComponent<SpriteComponent>("assets/pg1-Sheet.png", is_animated);
  player.addComponent<KeyboardController>();
  player.addComponent<ColliderComponent>("player", 0, 0, 32, 16, 0, 16);

  follower.addGroup(groupPlayers);
  follower2.addGroup(groupPlayers);
  player.addGroup(groupPlayers);
}

/**
 * Update the game
 */
void Game::update() {
  manager.refresh();
  manager.update();

  auto &pt = player.getComponent<TransformComponent>();

  // Usa il collider del player SOLO se presente, altrimenti nessuna collisione
  if (player.hasComponent<ColliderComponent>()) {
    SDL_Rect playerRect = player.getComponent<ColliderComponent>().collider;

    for (auto &collider : colliders) {
      const SDL_Rect cCol =
          collider->getComponent<ColliderComponent>().collider;

      if (Collision::AABB(playerRect, cCol)) {
        const float px = static_cast<float>(playerRect.x);
        const float py = static_cast<float>(playerRect.y);
        const float pw = static_cast<float>(playerRect.w);
        const float ph = static_cast<float>(playerRect.h);

        const float ox = static_cast<float>(cCol.x);
        const float oy = static_cast<float>(cCol.y);
        const float ow = static_cast<float>(cCol.w);
        const float oh = static_cast<float>(cCol.h);

        const float pCx = px + pw * 0.5f;
        const float pCy = py + ph * 0.5f;
        const float oCx = ox + ow * 0.5f;
        const float oCy = oy + oh * 0.5f;

        const float deltaX = pCx - oCx;
        const float deltaY = pCy - oCy;

        const float absDX = (deltaX < 0.0f) ? -deltaX : deltaX;
        const float absDY = (deltaY < 0.0f) ? -deltaY : deltaY;

        const float overlapX = (pw * 0.5f + ow * 0.5f) - absDX;
        const float overlapY = (ph * 0.5f + oh * 0.5f) - absDY;

        if (overlapX < overlapY) {
          const float push = (deltaX < 0.0f) ? -overlapX : overlapX;
          pt.position.x += push;
          playerRect.x += static_cast<int>(push);
          // opzionale: pt.velocity.x = 0.0f;
        } else {
          const float push = (deltaY < 0.0f) ? -overlapY : overlapY;
          pt.position.y += push;
          playerRect.y += static_cast<int>(push);
          // opzionale: pt.velocity.y = 0.0f;
        }
      }
    }
  }

  int halfWidth = int(camera.w / 2);
  int halfHeight = int(camera.h / 2);

  camera.x = pt.position.x - halfWidth;
  camera.y = pt.position.y - halfHeight;

  if (camera.x < 0) {
    camera.x = 0;
  }
  if (camera.y < 0) {
    camera.y = 0;
  }
  if (camera.x > camera.w) {
    camera.x = camera.w;
  }
  if (camera.y > camera.h) {
    camera.y = camera.h;
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

  for (auto &player : players) {
    player->draw();
  }

  if (showColliders) {
    for (auto &collider : colliders) {
      collider->draw();
    }

    if (player.hasComponent<ColliderComponent>()) {
      player.getComponent<ColliderComponent>().draw();
    }
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
  IMG_Quit();
  SDL_Quit();

  Utility::Log("Game cleaned");
}

/**
 * Handle events
 */
void Game::handleEvents() {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.repeat == 0) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        } else if (event.key.keysym.sym == SDLK_F1) {
          showColliders = !showColliders;
        }
      }
      break;
    default:
      break;
    }
  }
}