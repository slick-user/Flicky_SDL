#pragma once

#include <SDL3/SDL.h>
#include <iostream>

struct EditorCam {
  float x = 0;
  float y = 0;

  const float SPEED = 200.0f;

  void handleInput(float dt) {
  
    const bool* keys = SDL_GetKeyboardState(nullptr); 

    if (keys[SDL_SCANCODE_LEFT]) {
      x -= SPEED * dt;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
      x += SPEED * dt;
    }

    if (keys[SDL_SCANCODE_UP]) {
      y -= SPEED * dt;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
      y += SPEED * dt;
    }

}

};
