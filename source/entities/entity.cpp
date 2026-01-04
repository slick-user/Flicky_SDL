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

Facing atEdge(const SDL_FRect& a, const SDL_FRect& b) {
  // Check if enemy's left side is near platform's left edge
  bool nearLeftEdge = std::abs(a.x - b.x) <=0.1f;
    
  // Check if enemy's right side is near platform's right edge
  bool nearRightEdge = std::abs((a.x + a.w) - (b.x + b.w)) <= 0.1f;
  
  if (nearLeftEdge) return Facing::Left;
  else if (nearRightEdge) return Facing::Right;
  else return Facing::None;
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

        // ---- 1️⃣ PENETRATION-BASED COLLISION ----
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

        // ---- 2️⃣ EPSILON-BASED GROUND STICKINESS ----
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

