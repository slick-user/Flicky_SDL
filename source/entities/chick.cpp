#include <SDL3/SDL_timer.h>
#include <entities/chick.hpp>
#include <entities/player.hpp>

void Chick::updateState(float dt) {
  State newState = state;

  switch (state) {
    case State::Flying:
      // flies in sin wave motion vertical (up and down)
      v.x = 0.0f;
      v.y = std::sin(SDL_GetTicks() * 0.003f) * 10.0f;
      break;

    case State::Following:
      if (target) {
        const float FOLLOW_STRENGTH = 6.0f;

        float dx = target->x - x;
        float dy = target->y - y;

        x += dx * dt * FOLLOW_STRENGTH;
        y += dy * dt * FOLLOW_STRENGTH;
      }
      break;

    case State::Idle:
      // regular Idle state as in all entities
      break;
  }

  if (newState != state) 
    state = newState;
}

void Chick::update(float dt, const std::vector<Platform>& platforms) {
  if (state == State::Idle)
    Entity::update(dt, platforms);
  else {
    x += v.x * dt;
    y += v.y * dt;
  }

  updateState(dt);
  updateAnimation(dt, state);
}

void Chick::updateAnimation(float dt, State s) {

  switch (s) {
    case State::Idle: animator->play("idle"); break;
    case State::Following: animator->play("following"); break;
    case State::Flying: animator->play("flying"); break;
  }

  animator->update(dt);
}

void Chick::onCollision(Entity* e) {
  if (e->getEntityType() == std::string("Player")) {
    if (state == State::Flying) {
      Player* p = static_cast<Player*>(e);

      Chick* prev = p->getLastChick();
      int order = prev ? prev->index + 1 : 0;

      attach(p, prev, order);
      p->addChick(this);
    }
    // have them attach to the player
  }
  if (e->getEntityType() == std::string("Enemy")) {
    // if state is Following, detach from Player
    if (state == State::Following) {
      state = State::Flying;
      target = nullptr;
    }
  }
}

void Chick::attach(Player* p, Chick* prev, int order) {
  player = p;
  target = prev ? static_cast<Entity*>(prev) : dynamic_cast<Entity*>(p);
  index = order;
  state = State::Following;
  
  v.x = 0.0f;
  v.y = 0.0f;
  onGround = false;  
}

