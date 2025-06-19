#ifndef game_hpp
#define game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * Game class
 *
 * This class is used to create a game
 *
 * @author: @iMeyu
 */
class Game {
public:
  Game();
  ~Game();

  void init(const char *title, int xpos, int ypos, int width, int height,
            bool fullscreen);

  void handleEvents(); // Handle the events of the game
  void update();       // Update the game
  void render();       // Render the game
  void clean();        // Clean the game

  bool running() { return isRunning; }

private:
  bool isRunning;         // Whether the game is running
  SDL_Window *window;     // The window of the game
  SDL_Renderer *renderer; // The renderer of the game
};

#endif
