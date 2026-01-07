#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <entities/player.hpp>
#include <core/camera.hpp>
#include <core/renderer.hpp>
#include <core/game.hpp>
#include <entities/projectile.hpp>

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

    // Throwing Projectile
    if (projectile) {
      throwProjectile();
    }
  }

  if (keys[SDL_SCANCODE_DOWN] && projectile) {
    dropProjectile();
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

  if (e->getEntityType() == std::string("Projectile")) {
    Projectile* proj = dynamic_cast<Projectile*>(e);
    if (proj && proj->state == Projectile::State::Pickable && !projectile) {
      proj->pickUp(this);
      projectile = proj;
    }
  }
}

void Player::respawn() {
  x = originX;
  y = originY;
  v.x = 0.0f;
  v.y = 0.0f;
}

// Projectile Functions

void Player::tryPickUpProjectile() {
  if (projectile) return;

  SDL_FRect playerBounds = bounds();
}

void Player::throwProjectile() {
  if (!projectile) return;
  
  projectile->throwProjectile(facing);
  projectile = nullptr;
}

void Player::dropProjectile() {
  if (!projectile) return;
  
  projectile->drop();
  projectile = nullptr;
}