#pragma once
#include <stdio.h>

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"
#include <fstream>

#include "player.h"
//#include "enemy.h"
#include "util.h"

class Util;

// CONSTANT PARAMETERS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// DIMENSIONS OF LEVEL
#define LEVEL_WIDTH 1280
#define LEVEL_HEIGHT 480

// ANIMATION RELATED MACROS
#define PLAYER_IMAGE_HEIGHT 18
#define PLAYER_IMAGE_WIDTH 9

const int PLAYER_HEIGHT = PLAYER_IMAGE_HEIGHT * 2;
const int PLAYER_WIDTH = PLAYER_IMAGE_WIDTH * 2;

enum {
  IDLE,
  RUNNING,
};

class Game {
 
  private:
     // Properties
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Player* Player; 

    //Entity Enemy;

    Util img;

    //SDL_Rect Platform[11];
    SDL_Rect *Platform;
  
    bool initSDL();

    // temporary   
    SDL_Texture *background_texture = NULL;
    SDL_Rect *Flicky_Image = new SDL_Rect;
    SDL_Rect *Background_Image = new SDL_Rect;

    // Camera
    SDL_Rect camera = {0,0, LEVEL_WIDTH, LEVEL_HEIGHT};

  public:
    Game();

    void initPlatforms();
    void run();
    void kill();
 
};
