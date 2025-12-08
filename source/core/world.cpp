#include <SDL3_image/SDL_image.h>
#include <core/renderer.hpp>
#include <core/world.hpp>
#include <fstream>
#include <iostream>

World::World(SDL_Renderer* renderer) {
  std::string path = std::string(PROJECT_ROOT) + "/assets/Arcade - Flicky - Flicky.png";
  backGroundTexture = IMG_LoadTexture(renderer, path.c_str());

  if (!backGroundTexture) {
    std::cerr << "Failed to load background texture: " << path
              << "\nReason: " << SDL_GetError() << "\n";
  }
  else {
    std::cout << "Background Texture loaded!" << std::endl;
  }

  player = new Player(renderer, 20, 200, 18, 36);
  enemy = new Enemy(player, renderer, 0, 100, 18, 36);
  projectile = new Projectile(player, renderer, 0, 200, 12, 24);
}

World::~World() {
    delete player;
    delete enemy;
    delete projectile;
}

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
    player->update(platforms, 0);
    enemy->update(platforms, 0);
    projectile->update(platforms, 0);
}

void World::render(SDL_Renderer* renderer, Camera& cam) {
  
  SDL_FRect dst = {
    -cam.x,
    -cam.y,
    510.0f, // BACKRGOUND WIDTH
    446.0f  // BACKGROUND HEIGHT
  };
 
  SDL_RenderTexture(renderer, backGroundTexture, nullptr, &dst);

  //player->render(renderer);
    //enemy->render(renderer);
    //projectile->render(renderer);

  // For debugging: draw platforms
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  
    for (auto& p : platforms) {
    SDL_FRect rect = {
      (float)p.x,
      (float)p.y,
      (float)p.w,
      (float)p.h,
    };
    SDL_RenderRect(renderer, &rect);
  } 

}
