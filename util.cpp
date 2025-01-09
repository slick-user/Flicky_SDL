#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"
#include "include/SDL2/SDL_timer.h"

class Util {

  private:
  // ANIMATION RELATED MACROS
    #define PLAYER_IMAGE_HEIGHT 18
    #define PLAYER_IMAGE_WIDTH 9

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

Uint32 Util::update() {
  currentTime = SDL_GetTicks();
  
  return currentTime;
}

SDL_Texture* Util::load_texture(const char* texture_file, SDL_Renderer* renderer) {
  SDL_Surface *image = IMG_Load(texture_file); 
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 136, 136)); // Magenta as transparent 
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  return texture; 
} 

void Util::renderFrame(SDL_Renderer* renderer, SDL_Texture* spriteSheet, int animation, bool flipped, int frame, int x, int y) {

  SDL_Rect srcRect = { frame * PLAYER_IMAGE_WIDTH + 1, (animation * PLAYER_IMAGE_HEIGHT)*3, PLAYER_IMAGE_WIDTH, PLAYER_IMAGE_HEIGHT}; 

  if (frame == 1) {
    srcRect.x = 18; 
  }
  
  SDL_Rect destRect = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
  
  if (!flipped)
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, 0, NULL, SDL_FLIP_NONE);
  else if (flipped)
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void Util::updateAnimation() {
  if (currentTime > lastFrameTime + FRAME_DELAY) {
    currentFrame = (currentFrame + 1) % NUM_FRAMES;
    lastFrameTime = currentTime;
    //printf("%d", currentFrame);
  }
}
