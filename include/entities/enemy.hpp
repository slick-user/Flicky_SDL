#pragma once

#include <vector>
#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Player;

enum class EnemyState {
  Idle, 
  Walk,
  Jump,
  Fall
};

class Enemy : public Entity { 
  public:  
    SDL_Texture* texture = nullptr;
    spriteSheet* sprite = nullptr;

    EnemyState state;

    Enemy(float x, float y, Renderer& r, Player* p) : Entity(x,y,32,38) { 
      player = p;
      state = EnemyState::Idle;

      sheet = r.loadSpriteSheetJSON("Enemy", std::string(PROJECT_ROOT) + "/metadata/enemy.json");

      animator = new Animator();
      animator->setSheet(sheet);
      animator->play("idle");
  }

  virtual void ai(float dt, const std::vector<Platform>& platforms) = 0;

  virtual void updateState(float dt);
  virtual void updateAnimation(float dt, EnemyState s); 
  virtual void update(float dt, const std::vector<Platform>& platforms) override;


  protected:
    int frame = 0;
    float animTimer = 0.0f;
    float animSpeed = 0.0f;
    std::string spriteId;

    Player* player;

};
