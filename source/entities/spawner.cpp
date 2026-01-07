#include <entities/spawner.hpp>
#include <entities/nyannyan.hpp>
#include <core/world.hpp>

void Spawner::update(float dt, const std::vector<Platform>& platforms) {

  Entity::update(dt, platforms);
  updateState(dt);
  updateAnimation(dt, state);

  /* we could have made a timer custom class but to be honest we still have to 
    write the same amount of code if we did, it would be redundant, though there
    is a method for a timer could be worth consideration in the future */
  if (state == State::Spawning) {
    timer += dt;
    std::cout << timer << std::endl;

    if (timer >= SPAWN_DELAY) {
      spawn();
      timer = 0.0f;
    }
  }
  else
    timer = 0.0f;

}

void Spawner::updateAnimation(float dt, State s) {
  switch (s) {
    case State::Idle: animator->play("idle"); break;
    case State::Spawning: animator->play("spawning"); break;

    animator->update(dt);
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
    timer = 0.0f;
  }
}

void Spawner::spawn() {
  std::cout << "spawned!" << std::endl;
  world->entities.push_back(new NyanNyan(x,y, *world->r, player));
  enemy = dynamic_cast<NyanNyan*>(world->entities.back());
}
