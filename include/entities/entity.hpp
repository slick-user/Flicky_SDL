#pragma once

class Renderer;
class Camera;
class Animator;
struct spriteSheet;

const float JUMP_VELOCITY = 500.0;

constexpr float GRAVITY = 1200.0;
constexpr float MAX_FALL_SPEED = 900.0f;

constexpr float GROUND_EPS = 0.5f;

constexpr float COYOTE_TIME = 0.08f; 

static constexpr float JUMP_CUT_MULT = 0.4f;
const float SPEED = 150;

enum class Facing {
  Left,
  Right,
  None
};


#include <iostream>
#include <vector>
#include <SDL3/SDL.h>

bool intersects(const SDL_FRect& a, const SDL_FRect& b);
Facing atEdge(const SDL_FRect& a, const SDL_FRect& b);

struct Platform {
  SDL_FRect bounds;
};

class Entity {

public:
  float x = 0, y = 0;
  float w = 0, h = 0;

  SDL_FPoint v = {0.0f, 0.0f};
  bool onGround;

  Facing facing = Facing::Left;

  std::string textureId;
  SDL_Texture* texture;

  Animator* animator;
  spriteSheet* sheet;

public:

  Entity(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {
    onGround = false; 
    coyoteTimer = 0.0f;
  }

  virtual ~Entity() = default;  
  virtual void update(float dt, const std::vector<Platform>& platforms) = 0;
  virtual void render(Renderer& r, const Camera& cam);

  SDL_FRect bounds() const { return SDL_FRect {x,y,w,h}; }

  //virtual void updateAnimation(float dt) = 0;

  virtual void applyPhysics(float dt, const std::vector<Platform>& platforms);

private:
  void resolveVertical(const std::vector<Platform>& platforms);
  void resolveHorizontal(const std::vector<Platform>& platforms);

  void wrapHorizontal();

protected:
  float coyoteTimer;
};
