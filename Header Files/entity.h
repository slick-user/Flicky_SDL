#pragma once

#include "../include/SDL2/SDL_rect.h"
#include <iostream>
#include <vector>

#include "util.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int GROUND_LEVEL = SCREEN_HEIGHT - 70;
const int JUMP_VELOCITY = 15;

class Entity {

  protected:
    Util pImg;

    enum {
      IDLE,
      RUNNING,
    };

    SDL_Texture* texture;

    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path);

  public:

    bool can_jump = true;

    int vel[2] = {0,0};
    int* velocity = vel;

    int moved; //default is 0

    SDL_Rect p;
        
    Entity(SDL_Renderer* renderer, int x=20, int y=200, int w=18, int h=36);

    ~Entity();

    void checkCollision(const std::vector<SDL_Rect>& platform, const int platformCount);
    
    void update();
    void render(SDL_Renderer* renderer, SDL_Rect* Flicky_Image, Uint32 currentFrame);

};
