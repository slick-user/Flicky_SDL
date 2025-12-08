#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


  // ANIMATION RELATED MACROS
    #define PLAYER_IMAGE_HEIGHT 18
    #define PLAYER_IMAGE_WIDTH 9

class Util {

  private:

    const int PLAYER_HEIGHT = PLAYER_IMAGE_HEIGHT * 2;
    const int PLAYER_WIDTH = PLAYER_IMAGE_WIDTH * 2;

    const int NUM_FRAMES = 2;

    const Uint32 FRAME_DELAY = 100; // Milliseconds per frame
    
    Uint32 currentTime;

  public:
     int currentFrame = 0;
    Uint32 lastFrameTime = 0;

    
    // ANIMATION Functions
    Uint32 update(); 
    SDL_Texture* load_texture(const char* texture, SDL_Renderer* renderer);
    void renderFrame(SDL_Renderer* renderer, SDL_Texture* spriteSheet, int animation, bool flipped, int frame, int x, int y);
    void updateAnimation();

};
