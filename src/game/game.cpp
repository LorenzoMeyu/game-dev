#include "../game/game.hpp"
#include "../game/gameObject/game_object.hpp"
#include "../game/map/map.hpp"
#include <iostream>

SDL_Renderer *Game::renderer = nullptr; // The renderer of the game
GameObject *player;                     // The player object
Map *map;                               // The map object

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
    std::cout << "SDL_Init failed" << std::endl;
    isRunning = false;
    return;
  }

  // Create the window
  window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

  // Check if the window was created
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    isRunning = false;
    return;
  }

  std::cout << "Window created" << std::endl;

  // Create the renderer
  renderer = SDL_CreateRenderer(window, -1, 0);

  // Check if the renderer was created
  if (!renderer) {
    std::cout << "Failed to create renderer" << std::endl;
    isRunning = false;
    return;
  }

  std::cout << "Renderer created" << std::endl;

  // Set the renderer draw color to white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // Set the game to running
  isRunning = true;

  // Create a game object
  player = new GameObject("../assets/player.png", 190, 0, 64, 100);

  // Create a map object
  map = new Map();
}

/**
 * Update the game
 */
void Game::update() { player->Update(); }

/**
 * Render the game
 */
void Game::render() {
  // Clear the renderer
  SDL_RenderClear(renderer);

  // Draw the map
  map->DrawMap();

  // Draw the game object
  player->Render();

  // Present the renderer
  SDL_RenderPresent(renderer);
}

/**
 * Clean the game
 */
void Game::clean() {
  // Destroy the window and renderer
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  // Quit SDL
  SDL_Quit();

  std::cout << "Game cleaned" << std::endl;
}

/**
 * Handle events
 */
void Game::handleEvents() {
  // Create an event
  SDL_Event event;

  // Poll the event
  SDL_PollEvent(&event);

  // Check the type of the event
  switch (event.type) {
  // If the event is a quit event
  case SDL_QUIT:
    isRunning = false;
    break;
  default:
    break;
  }
}