#include <entities/enemy.hpp>

void Enemy::updateState(float dt) {
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

void Enemy::update(float dt, const std::vector<Platform>& platforms) {
  ai(dt, platforms);
  Entity::update(dt, platforms);
  updateState(dt);
  updateAnimation(dt, state);
}

void Enemy::updateAnimation(float dt, State s) {
 
  switch (s) {
    case State::Idle: animator->play("idle"); break;
    case State::Walk: animator->play("walk"); break;
    case State::Jump: animator->play("jump"); break;
    case State::Fall: animator->play("fall"); break;
  }

  animator->update(dt);  
}

void Enemy::onCollision(Entity* e) {
  if (e->getEntityType() == std::string("Player")) {
    // do nothing
  } 
}