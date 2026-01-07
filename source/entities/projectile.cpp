#include <entities/projectile.hpp>
#include <entities/player.hpp>
#include <entities/enemy.hpp>
#include <core/world.hpp>
#include <core/game.hpp>

void Projectile::update(float dt, const std::vector<Platform>& platforms) {
  switch (state) {
    case State::Pickable:
      Entity::update(dt, platforms);
      break;
      
    case State::Carried:
      // Follow player position
      if (carrier) {
        // Position relative to player
        x = carrier->x + (carrier->w - w) * 0.5f;
        y = carrier->y - h - 2.0f;  // Slightly above player
        v.x = 0.0f;
        v.y = 0.0f;
        onGround = false; // on Ground disables physics
      }
      break;
      
    case State::Thrown:
      Entity::update(dt, platforms);
      
      // Check if hit wall or went off screen
      if (x < 0 || x > LEVEL_WIDTH + 100) {
        auto& entities = world->entities;
        // Kills Enemy, (before that we will need to send the enemy flying)
        auto it = std::find(entities.begin(), entities.end(), this);
        if (it != entities.end()) {
          delete *it;
          entities.erase(it);
        }
      }
      break;
  }
  
  updateAnimation(dt);
}

void Projectile::pickUp(Player* player) {
  if (state != State::Pickable) return;
  
  state = State::Carried;
  carrier = player;
  v.x = 0.0f;
  v.y = 0.0f;
}

void Projectile::throwProjectile(Facing direction) {
  if (state != State::Carried || !carrier) return;
  
  state = State::Thrown;
  facing = direction;
  
  v.x = (direction == Facing::Left) ? -THROW_SPEED : THROW_SPEED;
  //v.y = THROW_VELOCITY_Y;  // Upward arc
  
  // Position at player's position when thrown
  x = carrier->x + (carrier->w - w) * 0.5f;
  y = carrier->y - h;
  
  carrier = nullptr;
}

void Projectile::drop() {
  if (state != State::Carried) return;
  
  state = State::Pickable;
  carrier = nullptr;
  v.x = 0.0f;
  v.y = 0.0f;
}

void Projectile::onCollision(Entity* other) {
  if (state != State::Thrown) return;
  
  // Hit enemy - stun or damage them
  if (other->getEntityType() == std::string("Enemy")) {
    Enemy* enemy = dynamic_cast<Enemy*>(other);
    if (enemy) {
      auto& entities = world->entities;
      // Kills Enemy, (before that we will need to send the enemy flying)
      auto it = std::find(entities.begin(), entities.end(), enemy);
      if (it != entities.end()) {
        delete *it;
        entities.erase(it);
      }
      
    }
  }
}

void Projectile::updateAnimation(float dt) {
  switch (state) {
    case State::Pickable:
      animator->play("idle");
      break;
    case State::Carried:
      animator->play("carried");
      break;
    case State::Thrown:
      animator->play("thrown");
      break;
  }
  animator->update(dt);
}