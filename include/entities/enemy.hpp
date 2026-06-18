#pragma once

#include <vector>
#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Player;

class Enemy : public Entity { 
  public:  
    SDL_Texture* texture = nullptr;
    spriteSheet* sprite = nullptr;

    State state;

    Enemy(float x, float y, Renderer& r, Player* p, EntityType type = EntityType::Enemy) : Entity(x,y,32,38, type) { 
      player = p;
      state = State::Idle;

      sheet = r.loadSpriteSheetJSON("Enemy", std::string(PROJECT_ROOT) + "/metadata/enemy.json");

      animator = new Animator();
      animator->setSheet(sheet);
      animator->play("idle");
  }

  virtual void ai(float dt, const std::vector<Platform>& platforms) = 0;

  virtual void updateState(float dt);
  virtual void updateAnimation(float dt, State s); 
  virtual void update(float dt, const std::vector<Platform>& platforms) override;

  void onCollision(Entity* other) override;
  virtual const char* getEntityType() const override { return "Enemy"; }
  virtual EntityType getEntityTypeEnum() const override { return EntityType::Enemy; }

  Player* getPlayer() { return player; }

  protected:
    int frame = 0;
    float animTimer = 0.0f;
    float animSpeed = 0.0f;
    std::string spriteId;

    Player* player;

};
