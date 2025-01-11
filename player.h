#pragma once

#include "include/SDL2/SDL_rect.h"
#include "include/SDL2/SDL_render.h"

#include "util.h"

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
   
    // Wil be for the x and y velocity or forces applied
    int velocity[2] = {0,0};

    SDL_Rect P;  // replace back with PLAYER_WIDTH AND PLAYER_HEIGHT 
 
    SDL_Texture *texture = nullptr;
    
    void init(SDL_Renderer *renderer);
    void update(int movement_x, int movement_y);           // For displaying or describing movement on the x and y axes
    void render(SDL_Renderer *renderer, SDL_Rect *Flicky_Image, Uint32 currentFrame, int moved);
};

