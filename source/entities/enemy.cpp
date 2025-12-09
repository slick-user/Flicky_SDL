/*#include "../../include/entities/enemy.hpp"

Enemy::Enemy(Player* player, SDL_Renderer* renderer, int x, int y, int w, int h) : Entity(renderer, x, y, w, h), player(player) {
 
  texture = loadTexture(renderer, "./assets/Arcade - Flicky - Flicky.png");

  jump();

} 

void Enemy::move() {
 
  if (dir == 1) {
    vel[0] = ENEMY_SPEED; 
    moved = 1;
  }
  else if (dir == 2) {
    vel[0] = -ENEMY_SPEED;
    moved = 2;
  }

}

void Enemy::jump() {
  vel[1] -= JUMP_VELOCITY;
  can_jump = false;
  stateUpdate = true;
}

void Enemy::updateState(int i) {
  if (i == 0) {
    if (player->p.x >= p.x) {
      dir = 1;
    }
    else {
      dir = 2;
    }
  }
  else {
    dir = i;
  }
}

void Enemy::checkCollision(const std::vector<SDL_Rect>& platform, const int platformCount) {
 
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
    std::cout << "GAME OVER! " << std::endl;
  }

  // Collision Check
  for (i=0; i<platformCount; i++) {

    if (SDL_HasRectIntersection(&p, &platform[i])) {
      CollisionSide side = getCollisionSide(oldPosition, p, platform[i]);
    
      switch (side) {
        
        case CollisionSide::TOP: {
          //landing on platform from below
          p.y = platform[i].y - p.h;
          vel[1] = 0;
          can_jump = true;

          const int buffer = 15;

          bool onLeft = (p.x + p.w >= platform[i].x - buffer && p.x + p.w <= platform[i].x);
          bool onRight = (p.x <= platform[i].x + platform[i].w + buffer && p.x >= platform[i].x + platform[i].w);

          bool isAbove = p.y > player->p.y;

          if (onLeft || onRight) {
            if (can_jump && isAbove) {
              jump();
            }
          }
         
          if (stateUpdate) {
            updateState();
            stateUpdate = false;
          }

          break;
        }
          
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
    else {
      const int buffer = 15;

      bool onLeft = (p.x + p.w >= platform[i].x - buffer && p.x + p.w <= platform[i].x);
      bool onRight = (p.x <= platform[i].x + platform[i].w + buffer && p.x >= platform[i].x + platform[i].w);
      
      bool isAbove = p.y > player->p.y;

      if (onLeft || onRight) {
        if (can_jump && isAbove) {
          jump();
        }
        else {
          updateState();
        }
      }

      if (stateUpdate) {
        updateState();
        stateUpdate = false;
      }
    }
  }

}
*/
