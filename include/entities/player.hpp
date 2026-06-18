#pragma once

//#include <util/headers.hpp>
#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Enemy;
class Chick;
class Projectile;

// PLAYER PROPERTIES

class Player : public Entity{

public:
  SDL_Texture* texture = nullptr;
  spriteSheet* sprite = nullptr;
  World* world = nullptr;
  
  Projectile* projectile = nullptr;
  std::vector<Chick*> chicks;

  State state;

  int lives = 3;

  Player(SDL_Texture* tex, float x, float y) : Entity(x,y,18,36, EntityType::Player), texture(tex) {}
  Player(float x, float y, Renderer& r) : Entity(x,y,18,36, EntityType::Player), originX(x), originY(y) {
    
    state = State::Idle;

    sheet = r.loadSpriteSheetJSON("Flicky", std::string(PROJECT_ROOT) + "/metadata/flicky.json");

    animator = new Animator();
    animator->setSheet(sheet);
    animator->play("idle");
  }

  void setWorld(World* w) { world = w; }
  void respawn();

  void updateState(float dt);

  void handleInput(float dt);
  void updateAnimation(float dt, State s);

  void update(float dt, const std::vector<Platform>& platforms) override;

  void onCollision(Entity* other) override;
  virtual const char* getEntityType() const override { return "Player"; }
  virtual EntityType getEntityTypeEnum() const override { return EntityType::Player; }
 
  void tryPickUpProjectile();
  void throwProjectile();
  void dropProjectile();

  void addChick(Chick* c);
  void removeChick(Chick* c);
  Chick* getLastChick();

public:
  int frame = 0;
  float animTimer = 0.0f;
  float animSpeed = 0.0f;
  std::string spriteId;

  float originX;
  float originY;

  bool isHit = false;
};

