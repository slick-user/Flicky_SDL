#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <memory>
#include <libs/json.hpp>

#include <core/camera.hpp>
//#include <entities/enemy.hpp>
#include <entities/player.hpp>
#include <entities/entity.hpp>
#include <entities/projectile.hpp>

class World {
public:
  World(int screenW, int screenH, Renderer& r);
  ~World();

  bool loadLevel(const std::string& filename);
  bool loadLevelViaTxt(const std::string& filename);

  void update(float dt);
  void cleanup();
  
  void startRespawnDelay();
  bool isRespawning() const { return respawnTimer > 0.0f; }

  Entity* spawnEntity(const std::string& type, float x, float y);
  Entity* addEntity(std::unique_ptr<Entity> e);
  void removeEntity(Entity* e);
  void removeEntityByID(EntityID id);
  Entity* findEntity(EntityID id);
  std::vector<std::unique_ptr<Entity>>& getEntities();

  Platform* findPlatform(PlatformID id);
  void removePlatformByID(PlatformID id);
  void resetIDCounters();

  void checkEntityCollisions();

  void onChickRescued();
  bool checkWinCondition() const;

  void addScore(int points) { score += points; }
  int getScore() const { return score; }
  int getTotalChicks() const { return totalChicks; }
  int getChicksExited() const { return chicksExited; }

private:

  float respawnTimer = 0.0f;
  const float RESPAWN_DELAY = 2.0f;

  int totalChicks = 0;
  int chicksExited = 0;
  int score = 0;

public:
  static PlatformID s_nextPlatformId;
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

  int platformCount = 0;
};
