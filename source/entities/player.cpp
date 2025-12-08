#include "../../include/entities/player.hpp"

Player::Player(SDL_Renderer* renderer, int x, int y, int w, int h) : Entity(renderer, x, y, w, h){

  texture = loadTexture(renderer, "./assets/Arcade - Flicky - Flicky.png");
  can_jump = true;
  projectile = nullptr;

} 

// GETTERS
bool Player::getPot() { return hasPot; }

// SETTERS
void Player::setPot(bool i) {
  hasPot = i;
}

void Player::setProjectile(Projectile* projectile) { this->projectile = projectile; } 

void Player::handleInput(const bool *state) {

  if ( state[SDL_SCANCODE_SPACE] && hasPot) {
    shoot();
    hasPot = false;
    std::cout << "work";
  }

  if (( state[SDL_SCANCODE_UP] && can_jump ) || ( state[SDL_SCANCODE_W] && can_jump )) {
    jump();
  }

  if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
    vel[0] = SPEED;
    moved = 1;
  }
  else if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
    vel[0] = -SPEED;
    moved = 2;
  }
  else {
    vel[0] = 0;
  }

}
/*
void Player::shoot() {
  projectile->setState(2);
  projectile->setDir(getMoved());
}
*/

void Player::shoot() {
  return;
}
