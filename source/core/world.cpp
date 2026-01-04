#include <core/renderer.hpp>
#include <core/world.hpp>
#include <entities/enemy.hpp>
#include <entities/nyannyan.hpp>
#include <entities/spawner.hpp>
#include <core/game.hpp>
#include <fstream>
#include <iostream>

World::World(int screenW, int screenH, Renderer& r) : camera(screenW, screenH), r(&r) {
  entities.push_back(new Player(20, 0, r));
  Player* ptr = dynamic_cast<Player*>(entities[0]);
  entities.push_back(new Spawner(500, 0, r, ptr, this));
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
        file >> platforms[i].bounds.x >> platforms[i].bounds.y >> platforms[i].bounds.w >> platforms[i].bounds.h;
    }
}

/*void World::loadLevel(const std::string& filename) {
  platforms.clear();
  entities.clear();

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to load level: " << filename << "\n";
    return;
  }

  Player* ptr;

  std::string type;
  while (file >> type) {
    if (type == "platform") {
      Platform p;
      file >> p.bounds.x >> p.bounds.y
           >> p.bounds.w >> p.bounds.h;
      platforms.push_back(p);
    }
    else if (type == "player") {
      float x, y;
      file >> x >> y;
      entities.push_back(new Player(x, y, *r));
      ptr = dynamic_cast<Player*>(entities[0]);
    }
    else if (type == "enemy") {
      float x, y;
      file >> x >> y;
      entities.push_back(new NyanNyan(x, y, *r, ptr));
    }
        
  }

  std::cout << "Level loaded: "
            << platforms.size() << " platforms\n";
}*/

void World::update(float dt) {
 
  for (auto& ent : entities)
    ent->update(dt, platforms);

  // Camera following player
  camera.follow(entities[0]);

  // Clamping Camera to level bounds
  camera.x = std::max(0.0f, std::min(camera.x, LEVEL_WIDTH - camera.width));
  camera.y = std::max(0.0f, std::min(camera.y, LEVEL_HEIGHT - camera.height));


}
