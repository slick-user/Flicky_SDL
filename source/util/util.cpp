#include "../../include/util/util.hpp"

Uint32 Util::update() {
  currentTime = SDL_GetTicks();
  
  return currentTime;
} 

// Renders the required frame
void Util::renderFrame(SDL_Renderer* renderer, SDL_Texture* spriteSheet, int animation, bool flipped, int frame, int x, int y)
{
    SDL_FRect srcF = {
        (float)(frame * PLAYER_WIDTH),
        (float)(animation * PLAYER_HEIGHT),
        (float)PLAYER_WIDTH,
        (float)PLAYER_HEIGHT
    };

    SDL_FRect dstF = {
        (float)x,
        (float)y,
        (float)PLAYER_WIDTH,
        (float)PLAYER_HEIGHT
    };

    SDL_FPoint center = { dstF.w * 0.5f, dstF.h * 0.5f };

    SDL_RenderTextureRotated(
        renderer,
        spriteSheet,
        &srcF,
        &dstF,
        0.0,                     // angle
        &center,                 // rotation center
        flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
    );
}

// Checks for which frame needs to be rendered basically
void Util::updateAnimation() {
  if (currentTime > lastFrameTime + FRAME_DELAY) {
    currentFrame = (currentFrame + 1) % NUM_FRAMES;
    lastFrameTime = currentTime;
  }
}

SDL_Texture* Util::load_texture(const char* texture_file, SDL_Renderer* renderer) {
  SDL_Surface *image = IMG_Load(texture_file); 
  //SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 136, 136)); // Cyan as transparent 
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_DestroySurface(image);

  return texture; 
}
