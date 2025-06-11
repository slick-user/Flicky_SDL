#include "../Header Files/entity.h"

Entity::Entity(SDL_Renderer* renderer, int x, int y, int w, int h) {;

  p.x = x;
  p.y = y;
  p.w = w;
  p.h = h;

  flickyImage->x = 1;
  flickyImage->y = 1;
  flickyImage->w = PLAYER_IMAGE_WIDTH;
  flickyImage->h = PLAYER_IMAGE_HEIGHT;

  texture = loadTexture(renderer, "./assets/Arcade - Flicky - Flicky.png");

}

Entity::~Entity() {
  SDL_DestroyTexture(texture);
  delete flickyImage;
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
  
  if (vel[1] > MAX_FALL_SPEED) vel[1] = MAX_FALL_SPEED;
  
  p.x += velocity[0];
  p.y += velocity[1];

}

void Entity::render(SDL_Renderer* renderer, Uint32 currentFrame, int cameraX, int cameraY) {

  SDL_Rect renderPos = {p.x - cameraX, p.y - cameraY, p.w, p.h};

  if (moved == 0){
    SDL_RenderCopy(renderer, texture, flickyImage, &renderPos);       // IDLE 
  }
  else if (moved == 1)
    pImg.renderFrame(renderer, texture, RUNNING, 0, currentFrame, p.x - cameraX, p.y - cameraY);
  else if (moved == 2) 
    pImg.renderFrame(renderer, texture, RUNNING, 1, currentFrame, p.x - cameraX, p.y - cameraY);

  moved = 0;
}

void Entity::checkCollision(const std::vector<SDL_Rect>& Platform, const int platformCount) {
 
  // storing old position for collision response
  SDL_Rect oldPosition = p;

  int i = 0;

  // Gravity  
  if (p.y < GROUND_LEVEL){
    vel[1] += 1;
  }
  else if (p.y > GROUND_LEVEL) {
    vel[1] = 0;
    can_jump = true;
    p.y = GROUND_LEVEL;
  }
  
  // Collision Check
  for (i=0; i<platformCount; i++) {

    if (SDL_HasIntersection(&p, &Platform[i])) {
      CollisionSide side = getCollisionSide(oldPosition, p, Platform[i]);
    
      switch (side) {
        
        case CollisionSide::TOP:
          //landing on platform from below
          p.y = Platform[i].y - p.h;
          vel[1] = 0;
          can_jump = true;
          //onPlatform = true;
          break;

        case CollisionSide::BOTTOM:
          //Hitting platform from below
          p.y = Platform[i].y + Platform[i].h;
          vel[1] = 0;
          break;

        case CollisionSide::LEFT:
        case CollisionSide::RIGHT:
          p.x = oldPosition.x;
          vel[0] = 0;
          break;
      }
    break;
    }

  }

}

CollisionSide Entity::getCollisionSide(const SDL_Rect& oldPos, const SDL_Rect& newPos, const SDL_Rect& platform) {
  int overlapX = std::min(newPos.x + newPos.w, platform.x + platform.w) - std::max(newPos.x, platform.x);
  int overlapY = std::min(newPos.y + newPos.h, platform.y + platform.h) - std::max(newPos.y, platform.y);

  if (overlapX <= 0 || overlapY <= 0) {
    return CollisionSide::TOP;
  }

  if (overlapX < overlapY) {
    // Horizontal Collision
    return (newPos.x < platform.x) ? CollisionSide::RIGHT : CollisionSide::LEFT;
  } 
  else {
    // Vertical Collision
    return (newPos.y < platform.y) ? CollisionSide::TOP : CollisionSide::BOTTOM;
  }
}

