#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <memory>

#include <core/camera.hpp>
//#include <entities/enemy.hpp>
#include <entities/player.hpp>
#include <entities/entity.hpp>
#include <entities/projectile.hpp>

class World {
public:
  World(int screenW, int screenH, Renderer& r);
  ~World();

  void loadLevel(const std::string& filename);

  void update(float dt);
  void cleanup();
  
  void startRespawnDelay();
  bool isRespawning() const { return respawnTimer > 0.0f; }

  Entity* addEntity(std::unique_ptr<Entity> e);
  
  void checkEntityCollisions();

  void onChickRescued();
  bool checkWinCondition() const;

private:

  int platformCount = 0;
  float respawnTimer = 0.0f;
  const float RESPAWN_DELAY = 2.0f;

  int totalChicks = 0;
  int chicksExited = 0;

public:
  std::vector<Platform> platforms;
  std::vector<std::unique_ptr<Entity>> entities;
  std::vector<std::unique_ptr<Entity>> pendingEntities;
  
  Player* player;
  Enemy* enemy;
  Projectile* projectile;
  Renderer* r;
  
  Camera camera;

  SDL_Texture* backgroundTexture = nullptr;
  SDL_FRect backgroundSrc{0,0,510,446};
  SDL_FRect backgroundDst{0,0,510,446};

  bool won = false; 
  bool lost = false; 

};
