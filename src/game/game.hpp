#ifndef game_hpp
#define game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class ColliderComponent;

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

  static SDL_Renderer *renderer; // The renderer of the game
  static SDL_Event event;        // The event of the game
  static bool isRunning;
  static SDL_Rect camera;
  static bool showColliders; // Whether to show colliders

  enum groupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupColliders,
  };

private:
  SDL_Window *window; // The window of the game
};

#endif
