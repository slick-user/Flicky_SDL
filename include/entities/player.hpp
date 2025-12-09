#pragma once

//#include <util/headers.hpp>
#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Enemy;
class Projectile;

// PLAYER PROPERTIES
const int GRAVITY = 2;
const int SPEED = 3;

class Projectile;

class Player : public Entity{

public:
  SDL_Texture* texture = nullptr;

  Player(SDL_Texture* tex, float x, float y) : Entity(x,y,18,36), texture(tex) {}
  Player(float x, float y, Renderer& r) : Entity(x,y,18,36) { 
    std::string path = std::string(PROJECT_ROOT) + "/assets/Arcade - Flicky - Flicky.png"; 
    texture = r.loadTexture("player", path);
    textureId = "player";
  }

  void update(float dt, const std::vector<SDL_FRect>& platforms) override;
  void render(Renderer& r, const Camera& cam) override;
};

