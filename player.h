#pragma once

#include "include/SDL2/SDL_rect.h"
#include "include/SDL2/SDL_render.h"
#include "include/SDL2/SDL_timer.h"
#include <iostream>

#include "util.h"

// CONSTANT PARAMETERS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// PLAYER PROPERTIES
#define GRAVITY 2;
#define SPEED 3

class Entity {

  private:
    Util p_img;

    enum {
      IDLE, 
      RUNNING,
    };

  public:
    int x = 20; 
    int y = 200;
    int w = 18;
    int h = 36;
   
  
    bool can_jump = true;

    // Wil be for the x and y velocity or forces applied
    int vel[2] = {0,0};
    int *velocity = vel; 

    int moved = 0;

    SDL_Rect P;  // replace back with PLAYER_WIDTH AND PLAYER_HEIGHT 
 
    SDL_Texture *texture = nullptr;
    
    void checkCollisions(SDL_Rect Platform[]);

    void init(SDL_Renderer *renderer);
    void handleInput(const Uint8 *state, SDL_Rect Platform[]);
    void update();           // For displaying or describing movement on the x and y axes
    void render(SDL_Renderer *renderer, SDL_Rect *Flicky_Image, Uint32 currentFrame);
  
};

