#include <entities/spawner.hpp>
#include <entities/nyannyan.hpp>
#include <core/world.hpp>

void Spawner::update(float dt, const std::vector<Platform>& platforms) {
  if (state == State::Spawning) {
    spawn();
  }

  Entity::update(dt, platforms);
  updateState(dt);
  updateAnimation(dt, state);

}

void Spawner::updateAnimation(float dt, State s) {
  switch (s) {
    case State::Idle: animator->play("idle"); break;
    case State::Spawning: animator->play("spawning"); break;
  }
}

void Spawner::updateState(float dt) {
  State newState = state;

  if (enemy != nullptr) 
    newState = State::Idle;
  else 
    newState = State::Spawning;

  if (newState != state) {
    state = newState;
  }
}

void Spawner::spawn() {
  world->entities.push_back(new NyanNyan(x,y, *world->r, player));
  enemy = dynamic_cast<NyanNyan*>(world->entities.back());
}
