#include "../../include/entities/entity.h"

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

// GETTERS
int Entity::getMoved() { return moved; }

SDL_Texture* Entity::loadTexture(SDL_Renderer* renderer, const char* path) {
 
  SDL_Texture* texture = pImg.load_texture(path, renderer); 

  if (!texture) {
    std::cout << "Unable to load image %s\n" <<  SDL_GetError();
    exit(1);
  }

  return texture;
} 

void Entity::update(const std::vector<SDL_Rect>& platform, const int platformCount) {
  
  moved = 0;

  if (vel[1] > MAX_FALL_SPEED) vel[1] = MAX_FALL_SPEED;
  
  p.x += vel[0];
  p.y += vel[1];

  checkCollision(platform, platformCount);

}

void Entity::render(SDL_Renderer* renderer, Uint32 currentFrame, int cameraX, int cameraY) {

  SDL_Rect renderPos = {p.x - cameraX, p.y - cameraY, p.w, p.h};

  if (moved == 0){
    SDL_FRect src = {
      (float)flickyImage->x,
      (float)flickyImage->y,
      (float)flickyImage->w,
      (float)flickyImage->h
    };

    SDL_FRect dst = {
      (float)renderPos.x,
      (float)renderPos.y,
      (float)renderPos.w,
      (float)renderPos.h
    };

    SDL_RenderTexture(renderer, texture, &src, &dst);       // IDLE 
  }
  else if (moved == 1)
    pImg.renderFrame(renderer, texture, RUNNING, 0, currentFrame, p.x - cameraX, p.y - cameraY);
  else if (moved == 2) 
    pImg.renderFrame(renderer, texture, RUNNING, 1, currentFrame, p.x - cameraX, p.y - cameraY);

  moved = 0;
}

void Entity::jump() {
  vel[1] -= JUMP_VELOCITY;
  can_jump = false;
}

void Entity::checkCollision(const std::vector<SDL_Rect>& platform, const int platformCount) {
 
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

    if (SDL_HasRectIntersection(&p, &platform[i])) {
      CollisionSide side = getCollisionSide(oldPosition, p, platform[i]);
    
      switch (side) {
        
        case CollisionSide::TOP:
          //landing on platform from below
          p.y = platform[i].y - p.h;
          vel[1] = 0;
          can_jump = true;
          //onPlatform = true;
          break;

        case CollisionSide::BOTTOM:
          //Hitting platform from below
          p.y = platform[i].y + platform[i].h;
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

Entity::~Entity() {
  SDL_DestroyTexture(texture);
  delete flickyImage;
}

