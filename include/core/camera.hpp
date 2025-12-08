#pragma once
#include <SDL3/SDL.h>

class Camera {
public:
  float x = 0.0f;
  float y = 0.0f;

  float width;
  float height;

  Camera(float x, float y, float width, float height) {}

  void follow(float targetX, float targetY) {
    x = targetX - width * 0.5f;
    y = targetY - height * 0.5f;
  }

};
