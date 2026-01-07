#pragma once

#include <vector>
#include <SDL3/SDL.h>

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

  void startRespawnDelay();
  bool isRespawning() const { return respawnTimer > 0.0f; }

private:

  int platformCount = 0;
  float respawnTimer = 0.0f;
  const float RESPAWN_DELAY = 2.0f;

  void checkEntityCollisions();

public:
  std::vector<Platform> platforms;
  std::vector<Entity*> entities;
  
  Player* player;
  Enemy* enemy;
  Projectile* projectile;
  Renderer* r;
  
  Camera camera;

  SDL_Texture* backgroundTexture = nullptr;
  SDL_FRect backgroundSrc{0,0,510,446};
  SDL_FRect backgroundDst{0,0,510,446};

};
