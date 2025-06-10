#pragma once

#include "../include/SDL2/SDL_rect.h"
#include <iostream>

#include "util.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Entity {

  protected:
    Util p_img;

    enum {
      IDLE,
      RUNNING,
    };

    SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string asset);

  public:

    bool can_jump = true;

    int vel[2] = {0,0};
    int* velocity = vel;

    int moved; //default is 0

    SDL_Rect p;
    
    SDL_Texture* texture;
    
    Entity(SDL_Renderer* renderer, int x=20, int y=200, int w=18, int h=36);

    void checkCollision(SDL_Rect Platform[]);
    
    void update();
    void render(SDL_Renderer* renderer, SDL_Rect* Flicky_Image, Uint32 currentFrame);

};
