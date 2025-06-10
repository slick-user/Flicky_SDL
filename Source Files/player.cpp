#include "../Header Files/player.h"

Player::Player(SDL_Renderer* renderer, int x, int y, int w, int h) : Entity(renderer, x, y, w, h){

  texture = loadTexture(renderer, "./assets/Arcade - Flicky - Flicky.png");

} 

void Player::handleInput(const Uint8 *state) {
 
  if (( state[SDL_SCANCODE_UP] && can_jump ) || ( state[SDL_SCANCODE_W] && can_jump )) {
    p.y -= 10;
    velocity[1] -= 15;
    can_jump = false;
  }

  if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
    velocity[0] = SPEED;
    moved = 1;
  }
  else if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
    velocity[0] = -SPEED;
    moved = 2;
  }
  else {
    velocity[0] = 0;
  }

}

