/*
#include "../../include/entities/projectile.hpp"

Projectile::Projectile(Player* player, SDL_Renderer* renderer, int x, int y, int w, int h) : Entity(renderer, x, y, w, h), player(player) {
  texture = loadTexture(renderer, "./assets/Custom Edited - Sonic the Hedgehog Customs - Flicky Sonic 1-Style Expanded.png");
}

void Projectile::setState(int i) {
  state = i;
}

void Projectile::setDir(int i) {
  dir = i;
}

void Projectile::update(const std::vector<SDL_Rect>& platform, const int platformCount) {
  
  moved = 0;

  if (state == 1) {
    p.x = player->p.x;
    p.y = player->p.y; 
  }

  if (state == 2) {  
    if (dir == 1) {
      vel[0] = SPEED; 
      moved = 1;
    }
    else if (dir == 2) {
      vel[0] = -SPEED;
      moved = 2;
    }  
  }

  if (vel[1] > MAX_FALL_SPEED) vel[1] = MAX_FALL_SPEED;
  
  p.x += vel[0];
  p.y += vel[1];

  checkCollision(platform, platformCount); 

}

void Projectile::checkCollision(const std::vector<SDL_Rect>& platform, const int platformCount) {
   
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
 
  
  // Player Collision
  if (SDL_HasRectIntersection(&p, &player->p)) {
    if (!player->getPot() && state == 0) {
      player->setPot(true);
      player->setProjectile(this);
      state = 1;
    }
  }

  // Enemy Collision
  
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

*/
