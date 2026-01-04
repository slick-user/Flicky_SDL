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

  State state;

  Player(SDL_Texture* tex, float x, float y) : Entity(x,y,18,36), texture(tex) {}
  Player(float x, float y, Renderer& r) : Entity(x,y,18,36) {
    
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

public:
  int frame = 0;
  float animTimer = 0.0f;
  float animSpeed = 0.0f;
  std::string spriteId;
};

