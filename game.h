#pragma once
#include <stdio.h>

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

#include "player.h"
#include "util.h"

// CONSTANT PARAMETERS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// ANIMATION RELATED MACROS
#define PLAYER_IMAGE_HEIGHT 18
#define PLAYER_IMAGE_WIDTH 9

const int PLAYER_HEIGHT = PLAYER_IMAGE_HEIGHT * 2;
const int PLAYER_WIDTH = PLAYER_IMAGE_WIDTH * 2;

// PLAYER PROPERTIES
#define GRAVITY 2;
#define SPEED 5

enum {
  IDLE,
  RUNNING,
};

class Game {
 
  private:
     // Properties
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Entity Player;
    bool movement[4] = {false, false, false, false};

    Util img;

    SDL_Rect Platform[11];
 
    bool initSDL();

    // temporary    
    //SDL_Texture *player_texture = NULL; 
    SDL_Texture *background_texture = NULL;
    SDL_Rect *Flicky_Image = new SDL_Rect;
    SDL_Rect *Background_Image = new SDL_Rect;

  public:
    Game() = default;

    int init();
    void initPlatforms(SDL_Rect Platform[]);
    void run();
    void kill();
 
};
