#pragma once
#include <SDL3/SDL.h>

class Camera {
public:
  float x = 0.0f;
  float y = 0.0f;

  float width = 640;
  float height = 480;

  Camera() {}
  Camera(int width, int height) : width(width), height(height) {}

  void follow(float targetX, float targetY) {
    x = targetX - width * 0.5f;
    y = targetY - height * 0.5f;
  }

  SDL_FRect apply(const SDL_FRect& worldRect) const {
    SDL_FRect r = worldRect;
    r.x -= x;
    r.y -= y;
    return r;
  }

};
