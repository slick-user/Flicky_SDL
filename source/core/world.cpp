#include <SDL3/SDL_rect.h>
#include <core/renderer.hpp>
#include <core/world.hpp>
#include <entities/enemy.hpp>
#include <entities/entity.hpp>
#include <entities/nyannyan.hpp>
#include <entities/projectile.hpp>
#include <entities/spawner.hpp>
#include <entities/chick.hpp>
#include <entities/entrance.hpp>
#include <core/game.hpp>
#include <fstream>
#include <iostream>
#include <libs/json.hpp>
#include <memory>
#include <type_traits>

World::World(int screenW, int screenH, Renderer& r) : camera(screenW, screenH), r(&r) {
  auto player = std::make_unique<Player>(20, 0, r);
  
  Player* ptr = player.get();
  ptr->setWorld(this);
  this->player = ptr;

  entities.push_back(std::move(player));
  entities.push_back(std::make_unique<Spawner>(500, 0, r, ptr, this));
  entities.push_back(std::make_unique<Chick>(700, 0, r));
  this->totalChicks++;
  entities.push_back(std::make_unique<Entrance>(900, 0, r, this));
  entities.push_back(std::make_unique<Projectile>(30, 0, r, this));
}

World::~World() {}

void World::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to load level: " << filename << "\n";
        return;
    }
    else {
      std::cout << "Level Loaded!" << std::endl; 
    }

    file >> platformCount;
    platforms.resize(platformCount);

    for (int i = 0; i < platformCount; i++) {
        file >> platforms[i].bounds.x >> platforms[i].bounds.y 
             >> platforms[i].bounds.w >> platforms[i].bounds.h;
    }
}

/*
void World::loadLevel(const std::string& path) {
  platforms.clear();
  entities.clear();

  json j = loadJson(path);

  //LEVEL_WIDTH = j["meta"]["width"];
  //LEVEL_HEIGHT = j["meta"]["height"];

  // Players
  auto& p = j["player"];
  player = static_cast<Player*>(spawnEntity("Player", p["x"], p["y"]));

  // Platforms
  for (auto& pl : j["platforms"]) {
    platforms.push_back({pl["x"], pl["y"],
                        pl["w"], pl["h"]});
  }

  // entities
  for (auto& e : j["entities"]) {
    spawnEntity(e["type"], e["x"], e["y"]);
  }
}
*/

void World::update(float dt) {

  // Win/Lose Conditions
  if (won) {
    std::cout << "THATS GAME !!!!" << std::endl;
    return;
  }
  if (lost) {
    std::cout << "GAMEOVER";
    // Initiate GameOver Screen
    return;
  }
    
  // Player Respawning (Might have Player handle this themselves)
  if (respawnTimer > 0.0f) {
    respawnTimer -= dt;
    if (respawnTimer <= 0.0f) {
      respawnTimer = 0.0f;
    }
    
    if (!entities.empty()) {
     Player* player = dynamic_cast<Player*>(entities[0].get());
      if (player) {
        player->respawn();
      }
    }

    return;
  }

  // Entity Updating
  checkEntityCollisions();

  for (auto& ent : entities)
    ent->update(dt, platforms);

  // Merge pending entities
  if (!pendingEntities.empty()) {
    for (auto& e : pendingEntities) {
      entities.push_back(std::move(e));
    }
    pendingEntities.clear();
  }

  // Camera following player
  if (!entities.empty()) {
    // entities[0] tends to be player that is why it is set to follow that entity
    camera.follow(entities[0].get());
  }

  // Clamping Camera to level bounds
  camera.x = std::max(0.0f, std::min(camera.x, LEVEL_WIDTH - camera.width));
  camera.y = std::max(0.0f, std::min(camera.y, LEVEL_HEIGHT - camera.height));

  cleanup();
}

void World::cleanup() {
  entities.erase(
    std::remove_if(entities.begin(), entities.end(),
      [](const std::unique_ptr<Entity>& e) { return !e->isActive(); }),
    entities.end());
}

// This is an almost useless function that is only used by the spawner, it can be repurposed to make the engine easier in the future 
Entity* World::addEntity(std::unique_ptr<Entity> e) {
  Entity* ptr = e.get();
  pendingEntities.push_back(std::move(e));
  return ptr;
}

void World::removeEntity(Entity* e) {
    auto it = std::remove_if(entities.begin(), entities.end(),
        [e](const std::unique_ptr<Entity>& ptr) {
            return ptr.get() == e;
        });
    entities.erase(it, entities.end());
}

std::vector<std::unique_ptr<Entity>>& World::getEntities() {
    return entities;
}

Entity* World::spawnEntity(const std::string& type, float x, float y) {
  if (type == "Player") {

    auto e = std::make_unique<Player>(x, y, *r);
    player = e.get();
    player->setWorld(this);
    entities.push_back(std::move(e));
  } 
  else if (type == "Spawner") {
    entities.push_back(std::make_unique<Spawner>(x, y, *r, player, this));
  } 
  else if (type == "Nyannyan") {
    addEntity(std::unique_ptr<NyanNyan>());
  }
  else if (type == "Chick") {
    totalChicks++;
    entities.push_back(std::make_unique<Chick>(x, y, *r));
  } 
  else if (type == "Entrance") {
    entities.push_back(std::make_unique<Entrance>(x, y, *r, this));
  }
  else if (type == "Projectile") {
    entities.push_back(std::make_unique<Projectile>(x, y, *r, this));
  }
  else {
    std::cerr << "Unknown Entity Type: " << type << std::endl;
    return nullptr;
  }

  return entities.back().get();
}

void World::checkEntityCollisions() {

  // For the Player to be invulnerable after being hit
  if (respawnTimer > 0.0f) return;

  for (int i=0; i<entities.size(); ++i) {
    if (!entities[i]) continue;

    SDL_FRect boundsA = entities[i]->bounds();

    for (int j=i + 1; j<entities.size(); ++j) {
      if (!entities[j]) continue;

      SDL_FRect boundsB = entities[j]->bounds();

      if (intersects(boundsA, boundsB)) {
        if (entities[i]->isActive() && entities[j]->isActive()) {
          entities[i]->onCollision(entities[j].get());
          entities[j]->onCollision(entities[i].get());
        }
      }
    }
  }
}

void World::startRespawnDelay() {
  if (respawnTimer > 0.0f) return;  // Already respawning
  respawnTimer = RESPAWN_DELAY;
}

bool World::checkWinCondition() const {
  return totalChicks > 0 && chicksExited >= totalChicks;
}

void World::onChickRescued() {
  chicksExited++;
  if (checkWinCondition()) {
    won = true;
    std::cout << "YOU WIN!!\n";
  }
}

