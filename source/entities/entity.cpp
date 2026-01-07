#include <entities/entity.hpp>
#include <core/renderer.hpp>
#include <core/camera.hpp>
#include <core/game.hpp>

bool intersects(const SDL_FRect& a, const SDL_FRect& b) {
  return !(a.x + a.w <= b.x ||  // a hpos is <= b hpos
           a.x >= b.x + b.w ||  // b hpos is >= a hpos
           a.y + a.h <= b.y ||  // a vpos is <= b vpos
           a.y >= b.y + b.h);   // a vpos is >= b vpos
}

Facing atEdge(const SDL_FRect& entity, const SDL_FRect& platform) {
  // Check to see if current platform is the one entity is standing on
  float footY = entity.y + entity.h;
  bool onPlatform = (footY >= platform.y - 2.0f) && 
                    (footY <= platform.y + 2.0f);
  
  if (!onPlatform) return Facing::None;
  
  // Check horizontal overlap
  bool overlaps = (entity.x < platform.x + platform.w) && 
                  (entity.x + entity.w > platform.x);
  if (!overlaps) return Facing::None;
  
  // Check if near left edge
  float distToLeft = std::abs(entity.x - platform.x);
  bool nearLeftEdge = distToLeft <= 5.0f;  // Increased tolerance
  
  // Check if near right edge  
  float distToRight = std::abs((entity.x + entity.w) - (platform.x + platform.w));
  bool nearRightEdge = distToRight <= 5.0f;
  
  if (nearLeftEdge) return Facing::Left;
  if (nearRightEdge) return Facing::Right;
  return Facing::None;
}

// Helper function to check if there's a platform ahead to land on
bool hasLandingPlatform(const SDL_FRect& entity, Facing direction, 
                          const std::vector<Platform>& platforms, float jumpDistance) {

  float checkX = (direction == Facing::Left) ? entity.x - jumpDistance : entity.x + entity.w + jumpDistance;

  float checkY = entity.y + entity.h + 50.0f;  // Check below current platform

  for (const auto& p : platforms) {
    // Check if platform is in jump range
    if (p.bounds.x <= checkX && checkX <= p.bounds.x + p.bounds.w) {
      float platformTop = p.bounds.y;
      float currentBottom = entity.y + entity.h;

      // Platform should be below but not too far
      if (platformTop > currentBottom && 
          platformTop - currentBottom < 200.0f) {
        return true;
      }
    }
  }
  return false;
}

bool isAtGap(const SDL_FRect& entity, Facing direction, 
             const std::vector<Platform>& platforms) {
  float checkX = (direction == Facing::Left) ? entity.x - 10.0f : entity.x + entity.w + 10.0f;
  float checkY = entity.y + entity.h + 5.0f;  // Just below feet

  // Check if there's a platform at this position
  for (const auto& p : platforms) {
    if (p.bounds.x <= checkX && checkX <= p.bounds.x + p.bounds.w &&
    std::abs(p.bounds.y - checkY) < 20.0f) {
    return false;  // Platform exists, not a gap
    }
  }
  return true;  // No platform, it's a gap
}

void Entity::render(Renderer& renderer, const Camera& cam) {
  int frame = animator->currentFrame();
  if (frame < 0 || frame >= (int)sheet->frames.size())
    return;

  SDL_FRect& src = sheet->frames[frame];
  SDL_FRect dst = cam.apply({std::floor(x),std::floor(y),w,h});

  bool flip = (facing == Facing::Left);

  renderer.drawSprite(sheet->texture, src, dst, flip); 
}

void Entity::update(float dt, const std::vector<Platform>& platforms) {
  applyPhysics(dt, platforms);
  wrapHorizontal();

  if (onGround)
    coyoteTimer = COYOTE_TIME;
  else
    coyoteTimer -= dt;
}

void Entity::applyPhysics(float dt, const std::vector<Platform>& platforms) {
  if (!onGround) {
    v.y += GRAVITY * dt;
    v.y = std::min(v.y, MAX_FALL_SPEED);
  }

  y += v.y * dt;
  resolveVertical(platforms);
  x += v.x * dt;
  resolveHorizontal(platforms);

  /* Debug
  static float lastX = x;
  if (std::abs(x - lastX) > 0.01f)
    std::cout << "x changed: " << x << "\n";
  lastX = x;
  */
}

void Entity::resolveVertical(const std::vector<Platform>& platforms) {
    bool groundedThisFrame = false;
    SDL_FRect box = bounds();

    for (const auto& p : platforms) {

        // Must overlap horizontally
        bool overlapX =
            box.x + box.w > p.bounds.x &&
            box.x < p.bounds.x + p.bounds.w;

        if (!overlapX)
            continue;

        if (intersects(box, p.bounds)) {

            if (v.y > 0) {
                // Falling → land
                y = p.bounds.y - h;
                v.y = 0;
                groundedThisFrame = true;
                box.y = y;
            }
            else if (v.y < 0) {
                // Jumping → hit ceiling
                y = p.bounds.y + p.bounds.h;
                v.y = 0;
                box.y = y;
            }
        }

        float footDist = (box.y + box.h) - p.bounds.y;

        if (v.y == 0 &&
            footDist >= -GROUND_EPS &&
            footDist <= GROUND_EPS) {

            groundedThisFrame = true;
            y = p.bounds.y - h;
            box.y = y;
        }
    }

    onGround = groundedThisFrame;
}

void Entity::resolveHorizontal(const std::vector<Platform>& platforms) {
    SDL_FRect box = bounds();

    for (const auto& p : platforms) {
        if (!intersects(box, p.bounds)) continue;

        if (v.x > 0)
            x = p.bounds.x - w;
        else if (v.x < 0)
            x = p.bounds.x + p.bounds.w;

        v.x = 0;
        box.x = x;
    }
}

void Entity::wrapHorizontal() {
  if (x + w < 0) {
    x = LEVEL_WIDTH;
  }
  else if (x > LEVEL_WIDTH) {
    x = -w;
  }
}

