#include <entities/enemy.hpp>
#include <entities/entity.hpp>
#include <entities/nyannyan.hpp>
#include <entities/player.hpp>
#include "../source/entities/entity.cpp"

void NyanNyan::ai(float dt, const std::vector<Platform>& platforms) {
  
  if (!onGround) {
    return;  // Let physics handle air movement
  }

  v.x = (facing == Facing::Left ? -SPEED : SPEED);

  SDL_FRect box = bounds();
  bool atLedge = false;
  Facing ledgeDirection = Facing::None;

  // find which platform Entity is on and check for ledge
  for (const auto& p : platforms) {
    Facing edge = atEdge(box, p.bounds);
    if (edge != Facing::None && edge == facing) {
      atLedge = true;
      ledgeDirection = edge;
      break;  // Found the ledge, no need to check more
    }
  }

  if (atLedge) {
    // Check if there's a platform to jump to
    bool canJumpToPlatform = hasLandingPlatform(box, facing, platforms, 150.0f);
    
    // Check if player is above and we should jump
    bool playerAbove = (player->y < y);
    
    // Decision: Jump if there's a platform OR if player is above
    if (canJumpToPlatform || playerAbove) {
      // Only jump if we haven't just jumped (cooldown)
      static float jumpCooldown = 0.0f;
      jumpCooldown -= dt;
      
      if (jumpCooldown <= 0.0f && onGround) {
        v.y = -JUMP_VELOCITY * 0.8f;  // Slightly reduced jump
        jumpCooldown = 0.5f;  // Prevent spam jumping
      }
    } else {
      // No platform ahead and player not above - turn around
      facing = (facing == Facing::Left) ? Facing::Right : Facing::Left;
      v.x = (facing == Facing::Left ? -SPEED : SPEED);
    }
  }
}
