#pragma once
#include <SDL3/SDL.h>
#include <entities/entity.hpp>

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

  void follow(Entity* e) {
    x = (e->x + e->w * 0.5f) - width * 0.5f;
    y = (e->y + e->h * 0.5f) - height * 0.5f;
  }

  SDL_FRect apply(const SDL_FRect& worldRect) const {
    SDL_FRect r = worldRect;
    r.x -= x;
    r.y -= y;
    return r;
  }

};
