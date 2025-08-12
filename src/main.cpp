#include "game/game.hpp"

// Create a game object
Game *game = nullptr;

int main(int argc, char *argv[]) {

  // Set the FPS and frame delay
  const int FPS = 60;
  const int frameDelay = 1000 / FPS;

  // Set the frame start and frame time variables for the game loop
  Uint32 frameStart;
  int frameTime;

  // Assign the game object to the game pointer
  game = new Game();

  game->init("TestGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
             640, false);

  // Start the game loop and run the game
  while (game->running()) {
    // Get the frame start time
    frameStart = SDL_GetTicks();

    // Handle events
    game->handleEvents();
    game->update();
    game->render();

    // Get the frame time
    frameTime = SDL_GetTicks() - frameStart;

    // If the frame time is less than the frame delay, delay the game
    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  game->clean();

  // Release the dynamically allocated Game instance
  delete game;
  game = nullptr;

  return 0;
}