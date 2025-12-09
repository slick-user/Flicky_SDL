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
  World(int screenW, int screenH);
  ~World();

  void loadLevel(const std::string& filename);

  void update(float dt);
  //void render(SDL_Renderer* renderer, Camera& cam);

private:

  int platformCount = 0;

  //later have to implement the remaining items

public:
  std::vector<SDL_FRect> platforms;
  std::vector<Entity*> entities;
  
  Player* player;
  Enemy* enemy;
  Projectile* projectile;
  
  Camera camera;

  SDL_Texture* backgroundTexture = nullptr;
  SDL_FRect backgroundSrc{0,0,510,446};
  SDL_FRect backgroundDst{0,0,510,446};

};
