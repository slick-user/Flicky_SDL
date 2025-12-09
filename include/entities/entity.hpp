#pragma once

#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <util/util.hpp>

class Renderer;
class Camera;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int GROUND_LEVEL = SCREEN_HEIGHT - 70;
const int JUMP_VELOCITY = 15;
const int MAX_FALL_SPEED = 10;

enum class CollisionSide {
  TOP, BOTTOM, LEFT, RIGHT
};

class Entity {

public:
  float x = 0, y = 0;
  float w = 0, h = 0;

  float vx = 0, vy = 0;

  std::string textureId;
  SDL_Texture* texture;

public:

  Entity(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

  virtual ~Entity() = default;
  
  virtual void update(float dt, const std::vector<SDL_FRect>& platforms) = 0;
  virtual void render(Renderer& r, const Camera& cam) = 0;

  SDL_FRect bounds() const { return SDL_FRect {x,y,w,h}; }
};
