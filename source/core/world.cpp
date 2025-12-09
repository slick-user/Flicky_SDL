#include <SDL3_image/SDL_image.h>
#include <core/renderer.hpp>
#include <core/world.hpp>
#include <fstream>
#include <iostream>
#include <memory>

World::World(int screenW, int screenH) : camera(screenW, screenH) {}

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
        file >> platforms[i].x >> platforms[i].y >> platforms[i].w >> platforms[i].h;
    }
}

void World::update(float dt) {
 
  /*
  for (auto& ent : entities)
    ent.update(dt, platforms);
  */
}
