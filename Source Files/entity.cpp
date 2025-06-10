#include "../Header Files/entity.h"

Entity::Entity(SDL_Renderer* renderer, int x, int y, int w, int h) {;

  p.x = x;
  p.y = y;
  p.w = w;
  p.h = h;

  texture = loadTexture(renderer, "./assets/Arcade - Flicky - Flicky.png");

}

Entity::~Entity() {
  SDL_DestroyTexture(texture);
}

SDL_Texture* Entity::loadTexture(SDL_Renderer* renderer, const std::string& path) {
  
  SDL_Texture* texture = pImg.load_texture("./assets/Arcade - Flicky - Flicky.png", renderer); 

  if (!texture) {
    std::cout << "Unable to load image %s\n" <<  IMG_GetError();
    exit(1);
  }

  return texture;
} 

void Entity::update() {
  p.x += velocity[0];
  p.y += velocity[1];
}

void Entity::render(SDL_Renderer* renderer, SDL_Rect* Flicky_Image, Uint32 currentFrame) {

  if (moved == 0){
    SDL_RenderCopy(renderer, texture, Flicky_Image, &p);       // IDLE 
  }
  else if (moved == 1)
    pImg.renderFrame(renderer, texture, RUNNING, 0, currentFrame, p.x, p.y);
  else if (moved == 2) 
    pImg.renderFrame(renderer, texture, RUNNING, 1, currentFrame, p.x, p.y);

  moved = 0;
}

void Entity::checkCollision(const std::vector<SDL_Rect>& Platform, const int platformCount) {
  
  bool on_platform = false; 
  int i = 0;

  // Collision Check
  for (i=0; i<platformCount; i++) {
    
    //upward collision
    /*if (P.x > Platform[i].x && P.x < Platform[i].x + Platform[i].w && P.y> Platform[i].y && P.y < Platform[i].y + Platform[i].h) {
      velocity[1] = 0;
      break;
    }*/

    //downwards collision
    if ( (p.x > Platform[i].x && p.x < Platform[i].x + Platform[i].w) && (p.y>= Platform[i].y && p.y<= Platform[i].y + Platform[i].h) ) {
      on_platform = true;
      break;
    } 
  }

  if (on_platform == true) {
    velocity[1] = 0;
    p.y = Platform[i].y; 
    can_jump = true;
  }
  else if (p.y < GROUND_LEVEL && on_platform == false) { // Falling state
    velocity[1] += 1;
  }
  else {
    velocity[1] = 0;
    p.y = GROUND_LEVEL;       // Is snapped to the ground
    can_jump = true;
  }
}

