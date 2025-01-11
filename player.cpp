#include <iostream>
#include "player.h"

void Entity::init(SDL_Renderer *renderer) {
  
  P.x = x;
  P.y = y;
  P.w = w;
  P.h = h;

  texture = p_img.load_texture("./assets/Arcade - Flicky - Flicky.png", renderer); 

  if (!texture) {
    std::cout << "Unable to load image %s\n" <<  IMG_GetError();
    exit(1);
  }


}

void Entity::update(int movement_x, int movement_y) {

  if (velocity[1] > 0) {
    velocity[1] = 0;
  }

  if (velocity[0] > 2) {
    velocity[0] = 0;
  }
  if (velocity[0] < -2) {
    velocity[0] = 0;
  }

  x += (movement_x + velocity[0]);
  y += (movement_y + velocity[1]);

  P.x = x;
  P.y = y;

}

void Entity::render(SDL_Renderer *renderer, SDL_Rect *Flicky_Image, Uint32 currentFrame, int moved) {
  if (moved == 0){
    SDL_RenderCopy(renderer, texture, Flicky_Image, &P);       // IDLE 
  }
  else if (moved == 1)
    p_img.renderFrame(renderer, texture, RUNNING, 0, currentFrame, P.x, P.y);
  else if (moved == 2) 
    p_img.renderFrame(renderer, texture, RUNNING, 1, currentFrame, P.x, P.y);

}
