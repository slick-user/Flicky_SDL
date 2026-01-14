#include <entities/entrance.hpp>
#include <entities/chick.hpp>
#include <core/world.hpp>

void Entrance::update(float dt, const std::vector<Platform>& platforms) {
  Entity::update(dt, platforms);
  updateState(dt);
  updateAnimation(dt, state);
}

void Entrance::updateAnimation(float dt, State s) {
  switch (s) {
    case State::Idle: animator->play("idle"); break;
    case State::Win: animator->play("win"); break;
  }

  animator->update(dt);
}

void Entrance::updateState(float dt) {
  State newState = state;

  if (world && world->won) {
    newState = State::Win;
  } else {
    newState = State::Idle;
  }

  if (newState != state)
    state = newState;

}

void Entrance::onCollision(Entity* e) {
  if (state == State::Win) return;  

  Chick* chick = dynamic_cast<Chick*>(e);
  if (!chick) return;

  if (chick->state == Chick::State::Following) {
    chick->state = Chick::State::Idle;
    world->onChickRescued();
  }
  
}
