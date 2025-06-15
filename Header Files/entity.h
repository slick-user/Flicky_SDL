#pragma once

#include "../include/SDL2/SDL_rect.h"
#include <iostream>
#include <vector>

#include "util.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int GROUND_LEVEL = SCREEN_HEIGHT - 70;
const int JUMP_VELOCITY = 15;
const int MAX_FALL_SPEED = 10;

enum class CollisionSide {
  TOP, BOTTOM, LEFT, RIGHT
};

class Entity {

  protected:
    Util pImg;
 
    SDL_Rect *flickyImage = new SDL_Rect;

    bool can_jump = true;
    
    float vel[2] = {0,0};

    int moved; //default is 0

    enum {
      IDLE,
      RUNNING,
    };

    SDL_Texture* texture;

    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path);

    CollisionSide getCollisionSide(const SDL_Rect& oldPos, const SDL_Rect& newPos, const SDL_Rect& platform);  

    virtual void jump();

  public:

    SDL_Rect p;
        
    Entity(SDL_Renderer* renderer, int x=20, int y=200, int w=18, int h=36);

    virtual ~Entity();

    virtual void checkCollision(const std::vector<SDL_Rect>& platform, const int platformCount);
    
    void update(const std::vector<SDL_Rect>& platform, const int platformCount);
    void render(SDL_Renderer* renderer, Uint32 currentFrame, int cameraX, int cameraY);

};
