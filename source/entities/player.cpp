#include <SDL3/SDL.h>
#include <entities/player.hpp>
#include <core/camera.hpp>
#include <core/renderer.hpp>

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
    PlayerState newState = state;

    if (!onGround)
        newState = (v.y < 0) ? PlayerState::Jump : PlayerState::Fall;
    else if (std::abs(v.x) > 0.01f)
        newState = PlayerState::Walk;
    else
        newState = PlayerState::Idle;

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

void Player::updateAnimation(float dt, PlayerState s) {
 
  switch (s) {
    case PlayerState::Idle: animator->play("idle"); break;
    case PlayerState::Walk: animator->play("walk"); break;
    case PlayerState::Jump: animator->play("jump"); break;
    case PlayerState::Fall: animator->play("fall"); break;
  }

  animator->update(dt);  

}

