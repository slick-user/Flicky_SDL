#include <entities/enemy.hpp>

void Enemy::updateState(float dt) {
    EnemyState newState = state;

    if (!onGround)
        newState = (v.y < 0) ? EnemyState::Jump : EnemyState::Fall;
    else if (std::abs(v.x) > 0.01f)
        newState = EnemyState::Walk;
    else
        newState = EnemyState::Idle;

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

void Enemy::updateAnimation(float dt, EnemyState s) {
 
  switch (s) {
    case EnemyState::Idle: animator->play("idle"); break;
    case EnemyState::Walk: animator->play("walk"); break;
    case EnemyState::Jump: animator->play("jump"); break;
    case EnemyState::Fall: animator->play("fall"); break;
  }

  animator->update(dt);  
}

