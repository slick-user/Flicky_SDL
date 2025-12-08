#pragma once

#include <vector>
#include <SDL3/SDL.h>

#include <core/camera.hpp>
#include <entities/enemy.hpp>
#include <entities/player.hpp>
#include <entities/entity.hpp>
#include <entities/projectile.hpp>

class World {
public:
  explicit World(SDL_Renderer* renderer);
  ~World();

  void loadLevel(const std::string& filename);

  void update(float dt);
  void render(SDL_Renderer* renderer, Camera& cam);

private:

  int platformCount = 0;

  //later have to implement the remaining items

public:
  std::vector<SDL_Rect> platforms;
 
  Player* player;
  Enemy* enemy;
  Projectile* projectile;

private:
  SDL_Texture* backGroundTexture = nullptr;
};
