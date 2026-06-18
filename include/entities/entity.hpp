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

enum class State {
  Idle,
  Walk,
  Jump,
  Fall
};

#include <vector>
#include <iostream>  // IDE says iostream is not used here, ignore it
#include <string>
#include <array>
#include <SDL3/SDL.h>

bool intersects(const SDL_FRect& a, const SDL_FRect& b);
Facing atEdge(const SDL_FRect& a, const SDL_FRect& b);

using PlatformID = uint32_t;
constexpr PlatformID INVALID_PLATFORM_ID = 0;

struct Platform {
  PlatformID id = INVALID_PLATFORM_ID;
  SDL_FRect bounds;
};

// ---- Entity ID System ----

enum class EntityType : uint8_t {
  Player    = 1,
  Chick     = 2,
  Spawner   = 3,
  Entrance  = 4,
  Projectile= 5,
  Enemy     = 6,
  NyanNyan  = 7,
  Entity    = 0
};

using EntityID = uint32_t;

constexpr EntityID INVALID_ID = 0;

inline EntityID makeID(EntityType type, uint32_t index) {
  return (static_cast<uint32_t>(type) << 24) | (index & 0x00FFFFFF);
}

inline EntityType getTypeFromID(EntityID id) {
  return static_cast<EntityType>((id >> 24) & 0xFF);
}

inline uint32_t getIndexFromID(EntityID id) {
  return id & 0x00FFFFFF;
}

inline const char* entityTypeToString(EntityType t) {
  switch (t) {
    case EntityType::Player:    return "Player";
    case EntityType::Chick:     return "Chick";
    case EntityType::Spawner:   return "Spawner";
    case EntityType::Entrance:  return "Entrance";
    case EntityType::Projectile:return "Projectile";
    case EntityType::Enemy:     return "Enemy";
    case EntityType::NyanNyan:  return "NyanNyan";
    default:                    return "Entity";
  }
}

// ---- Entity Class ----

class Entity {

public:
  float x = 0, y = 0;
  float w = 0, h = 0;

  SDL_FPoint v = {0.0f, 0.0f};
  bool onGround;
  bool active = true;

  Facing facing = Facing::Left;

  std::string textureId;
  SDL_Texture* texture;

  Animator* animator;
  spriteSheet* sheet;

  EntityID id = INVALID_ID;

  static std::array<uint32_t, 256> s_nextId;

public:

  Entity(float x, float y, float w, float h, EntityType type = EntityType::Entity) : x(x), y(y), w(w), h(h) {
    onGround = false; 
    coyoteTimer = 0.0f;
    uint8_t t = static_cast<uint8_t>(type);
    id = makeID(type, s_nextId[t]++);
  }

  void destroy() { active = false; }
  bool isActive() const { return active; }

  virtual ~Entity() = default;  
  virtual void update(float dt, const std::vector<Platform>& platforms) = 0;
  virtual void render(Renderer& r, const Camera& cam);

  SDL_FRect bounds() const { return SDL_FRect {x,y,w,h}; }

  //virtual void updateAnimation(float dt) = 0;

  virtual void applyPhysics(float dt, const std::vector<Platform>& platforms);

  virtual void onCollision(Entity* other) {}

  virtual const char* getEntityType() const { return "Entity"; }
  virtual EntityType getEntityTypeEnum() const { return EntityType::Entity; }

private:
  void resolveVertical(const std::vector<Platform>& platforms);
  void resolveHorizontal(const std::vector<Platform>& platforms);

  void wrapHorizontal();

protected:
  float coyoteTimer;
};
