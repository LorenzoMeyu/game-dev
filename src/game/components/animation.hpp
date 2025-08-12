#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "SDL2/SDL.h"

struct Animation {
  int index;
  int frames;
  int speed;

  Animation() {}
  Animation(int index, int frames, int speed) {
    this->index = index;
    this->frames = frames;
    this->speed = speed;
  }
};

#endif