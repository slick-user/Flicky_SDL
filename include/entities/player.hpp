#pragma once

//#include <util/headers.hpp>
#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Enemy;
class Projectile;

// PLAYER PROPERTIES

class Projectile;

class Player : public Entity{

public:
  SDL_Texture* texture = nullptr;
  spriteSheet* sprite = nullptr;
  World* world = nullptr;

  State state;

  int lives = 3;

  Player(SDL_Texture* tex, float x, float y) : Entity(x,y,18,36), texture(tex) {}
  Player(float x, float y, Renderer& r) : Entity(x,y,18,36), spawnX(x), spawnY(y) {
    
    state = State::Idle;

    sheet = r.loadSpriteSheetJSON("flicky", std::string(PROJECT_ROOT) + "/metadata/flicky.json");

    animator = new Animator();
    animator->setSheet(sheet);
    animator->play("idle");
  }

  void updateState(float dt);

  void handleInput(float dt);
  void updateAnimation(float dt, State s);

  void update(float dt, const std::vector<Platform>& platforms) override;

  void onCollision(Entity* other) override;
  void respawn();
  virtual const char* getEntityType() const override { return "Player"; }

  void setWorld(World* w) { world = w; }

public:
  int frame = 0;
  float animTimer = 0.0f;
  float animSpeed = 0.0f;
  std::string spriteId;

  float spawnX;
  float spawnY;

  bool isHit = false;
};

