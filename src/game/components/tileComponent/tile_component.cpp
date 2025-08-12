#include "tile_component.hpp"

TileComponent::TileComponent(int srcX, int srcY, int xpos, int ypos, int tile_size, int tile_scale, const char *path) {
    texture = TextureManager::LoadTexture(path);

    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = srcRect.h = tile_size;

    position.x = xpos;
    position.y = ypos;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = destRect.h = tile_size * tile_scale;
}

TileComponent::~TileComponent() {
    SDL_DestroyTexture(texture);
}

void TileComponent::update() {
    destRect.x = position.x - Game::camera.x;
    destRect.y = position.y - Game::camera.y;
}

void TileComponent::draw() {
    TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}