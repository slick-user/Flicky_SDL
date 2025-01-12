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

void Entity::update() { 
  
  P.x += velocity[0];
  P.y += velocity[1];
 
}

void Entity::handleInput(const Uint8 *state, SDL_Rect Platform[]) {
 
  checkCollisions(Platform);

  if (state[SDL_SCANCODE_UP] && can_jump) {
    velocity[1] -= 20;
    can_jump = false;
  }

  if (state[SDL_SCANCODE_RIGHT]) {
    velocity[0] = SPEED;
    moved = 1;
  }
  else if (state[SDL_SCANCODE_LEFT]) {
    velocity[0] = -SPEED;
    moved = 2;
  }
  else {
    velocity[0] = 0;
  }
  
}

void Entity::render(SDL_Renderer *renderer, SDL_Rect *Flicky_Image, Uint32 currentFrame) {
  if (moved == 0){
    SDL_RenderCopy(renderer, texture, Flicky_Image, &P);       // IDLE 
  }
  else if (moved == 1)
    p_img.renderFrame(renderer, texture, RUNNING, 0, currentFrame, P.x, P.y);
  else if (moved == 2) 
    p_img.renderFrame(renderer, texture, RUNNING, 1, currentFrame, P.x, P.y);

  moved = 0;
}

void Entity::checkCollisions(SDL_Rect Platform[]) {
  
  bool on_platform = false;
  
  int platform_pos;

  // Collision Check
  for (int i=0; i<11; i++) {
    if ( (P.x > Platform[i].x && P.x < Platform[i].x + Platform[i].w) && (P.y > Platform[i].y && P.y < Platform[i].y + Platform[i].h) ) {
      on_platform = true;
      platform_pos = Platform[i].y;
      break;
    }
  }

  if (on_platform == true) {
    velocity[1] = 0;
    P.y = platform_pos; 
    can_jump = true;
  }
  else if (P.y < SCREEN_HEIGHT - 70 && on_platform == false) { // Falling state
    velocity[1] += 1;
  }
  else {
    velocity[1] = 0;
    P.y = SCREEN_HEIGHT - 70;       // Is snapped to the ground
    can_jump = true;
  }
}
