#include "../Header Files/util.h"

Uint32 Util::update() {
  currentTime = SDL_GetTicks();
  
  return currentTime;
} 

// Renders the required frame
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

// Checks for which frame needs to be rendered basically
void Util::updateAnimation() {
  if (currentTime > lastFrameTime + FRAME_DELAY) {
    currentFrame = (currentFrame + 1) % NUM_FRAMES;
    lastFrameTime = currentTime;
  }
}

SDL_Texture* Util::load_texture(const char* texture_file, SDL_Renderer* renderer) {
  SDL_Surface *image = IMG_Load(texture_file); 
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 136, 136)); // Cyan as transparent 
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  return texture; 
}
