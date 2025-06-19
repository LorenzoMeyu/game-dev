#include "game_object.hpp"
#include "../textureManager/texture_manager.hpp"

/**
 * Constructor for the GameObject class
 *
 * @param textureSheet The path to the texture sheet
 * @param ren The renderer to use
 */
GameObject::GameObject(const char *textureSheet, SDL_Renderer *ren, int x,
                       int y) {
  renderer = ren;
  objTexture = TextureManager::LoadTexture(textureSheet, renderer, 64, 100);
  xpos = x;
  ypos = y;
}

GameObject::~GameObject() { SDL_DestroyTexture(objTexture); }

void GameObject::Update() {
  xpos += 1;
  ypos += 1;

  srcRect.h = 100; // The height of the source rectangle
  srcRect.w = 64;  // The width of the source rectangle
  srcRect.x = 0;   // The x position of the source rectangle
  srcRect.y = 0;   // The y position of the source rectangle

  destRect.x = xpos;      // The x position of the destination rectangle
  destRect.y = ypos;      // The y position of the destination rectangle
  destRect.h = srcRect.h; // The height of the destination rectangle
  destRect.w = srcRect.w; // The width of the destination rectangle
}

/**
 * Render the game object
 */
void GameObject::Render() {
  SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}