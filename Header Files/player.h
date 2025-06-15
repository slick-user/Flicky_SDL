#pragma once

#include "../include/SDL2/SDL_rect.h"
#include "../include/SDL2/SDL_render.h"
#include "../include/SDL2/SDL_timer.h"
#include <iostream>

#include "util.h"
#include "../Header Files/entity.h"

// PLAYER PROPERTIES
const int GRAVITY = 2;
const int SPEED = 3;

class Player : public Entity{

  private:

  public:
 
    Player(SDL_Renderer* renderer, int x=20, int y=200, int w=18, int h=36);

    void handleInput(const Uint8 *state);
  
};

