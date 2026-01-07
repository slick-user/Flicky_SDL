#include <SDL3/SDL.h>
#include <entities/player.hpp>
#include <core/camera.hpp>
#include <core/renderer.hpp>
#include <core/game.hpp>

void Player::handleInput(float dt) {
  
  const bool* keys = SDL_GetKeyboardState(nullptr); 

  float inputX = 0.0f;

  if (keys[SDL_SCANCODE_LEFT]) {
    inputX -= 1.0f;
  }
  if (keys[SDL_SCANCODE_RIGHT]) {
    inputX += 1.0f;
  }

  v.x = inputX * SPEED;

  if (inputX < 0)
    facing = Facing::Left;
  else if (inputX > 0)
    facing = Facing::Right;

  if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP]) && onGround && coyoteTimer > 0.0f) {
    v.y = -JUMP_VELOCITY;
    onGround = false;
    coyoteTimer = 0.0f;
  }
  
  // Variable jump height
  if (!(keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP]) && v.y < 0) {
        v.y *= JUMP_CUT_MULT;
  }

}

void Player::updateState(float dt) {
    State newState = state;

    if (!onGround)
        newState = (v.y < 0) ? State::Jump : State::Fall;
    else if (std::abs(v.x) > 0.01f)
        newState = State::Walk;
    else
        newState = State::Idle;

    if (newState != state) {
        state = newState;
    }
}

void Player::update(float dt, const std::vector<Platform>& platforms) {

  handleInput(dt);                
  Entity::update(dt, platforms);
  updateState(dt);
  updateAnimation(dt, state);
}

void Player::updateAnimation(float dt, State s) {
 
  switch (s) {
    case State::Idle: animator->play("idle"); break;
    case State::Walk: animator->play("walk"); break;
    case State::Jump: animator->play("jump"); break;
    case State::Fall: animator->play("fall"); break;
  }

  animator->update(dt);  

}

void Player::onCollision(Entity* e) {
  if (e->getEntityType() == std::string("Enemy")) {
    lives -= 1;
    
    if (world) {
      world->startRespawnDelay();
    }
  } 
}

void Player::respawn() {
  x = spawnX;
  y = spawnY;
  v.x = 0.0f;
  v.y = 0.0f;
}
