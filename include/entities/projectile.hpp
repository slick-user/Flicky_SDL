#pragma once

#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Player;
class World;

class Projectile : public Entity {
public:
  enum class State {
    Pickable,    // On ground, can be picked up
    Carried,     // Being carried by player
    Thrown       // Flying through air
  };

  State state = State::Pickable;
  Player* carrier = nullptr;  // Player carrying this projectile
  World* world = nullptr;
  
  const float THROW_SPEED = 400.0f;  // Horizontal throw speed
  const float THROW_VELOCITY_Y = -300.0f;  // Upward throw velocity
  
  Projectile(float x, float y, Renderer& r, World* w) 
    : Entity(x, y, 16, 16), world(w) {
    
    // Load projectile sprite sheet
    sheet = r.loadSpriteSheetJSON("Projectile", 
      std::string(PROJECT_ROOT) + "/metadata/projectile.json");
    
    animator = new Animator();
    animator->setSheet(sheet);
    animator->play("idle");
  }
  
  void update(float dt, const std::vector<Platform>& platforms) override;
  void onCollision(Entity* other) override;
  const char* getEntityType() const override { return "Projectile"; }
  
  void pickUp(Player* player);
  void throwProjectile(Facing direction);
  void drop();
  
private:
  void updateState(float dt);
  void updateAnimation(float dt);
};