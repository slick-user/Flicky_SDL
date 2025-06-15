#pragma once

#include "../include/SDL2/SDL_rect.h"
#include "../include/SDL2/SDL_render.h"
#include "../include/SDL2/SDL_timer.h"
#include <iostream>

#include "../Header Files/entity.h"
#include "../Header Files/player.h"
#include "util.h"

// CONSTANT PARAMETERS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// PLAYER PROPERTIES
#define GRAVITY 2;
#define SPEED 3

class Enemy : public Entity { 
  
  protected:

    Player* player;

    int dir = 1;
    bool stateUpdate = false;

  public:

    Enemy(Player* player, SDL_Renderer* renderer, int x=20, int y=200, int w=18, int h=36);  

    //void checkPlayerCollision();
    void move();
    void jump();
    void updateState(int i=0);
    void checkCollision(const std::vector<SDL_Rect>& platform, const int platformCount);

};
