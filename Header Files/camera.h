#pragma once

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"
#include <iostream>
#include <fstream>

#include "../Header Files/player.h"

class Camera {
  private:
    SDL_Rect viewport;
    float offsetX;
    int levelWidth;

  public:
    Camera(int screenWidth, int screenHeight, int levelWidth);
  
    void update(const Player& target);

    SDL_Rect getViewport() const;

    SDL_Rect worldToScreen(const SDL_Rect& worldRect) const;
  
};
