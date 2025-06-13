#pragma once
#include <stdio.h>

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"
#include <fstream>
#include <vector>

#include "player.h"
//#include "enemy.h"
#include "util.h"

class Util;

// DIMENSIONS OF LEVEL

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 480;

const int PLAYER_HEIGHT = PLAYER_IMAGE_HEIGHT * 2;
const int PLAYER_WIDTH = PLAYER_IMAGE_WIDTH * 2;

// CAMERA CONSTANTS
const int LEFT_WRAP_BOUNDS = 320;
const int RIGHT_WRAP_BOUNDS = LEVEL_WIDTH - 320;

const int CAMERA_SCROLL_SPEED = 10;

enum {
  IDLE,
  RUNNING,
};

class Game {
 
  private:
     // Properties
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Player* player; 

    //Entity Enemy;

    Util img;

    int platformCount;
    std::vector<SDL_Rect> platform;
  
    bool initSDL();

    // temporary   
    SDL_Texture *backgroundTexture = NULL;
    SDL_Rect *backgroundImage = new SDL_Rect;

    // Camera
    SDL_Rect camera = {0,0, LEVEL_WIDTH, LEVEL_HEIGHT};

  public:
    Game();
    ~Game();

    void initPlatforms();
    void run();

    void updateCamera();
 
};
