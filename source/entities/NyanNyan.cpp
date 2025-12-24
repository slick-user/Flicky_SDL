#include <entities/enemy.hpp>
#include <entities/entity.hpp>
#include <entities/nyannyan.hpp>
#include <entities/player.hpp>

void NyanNyan::ai(float dt, const std::vector<Platform>& platforms) {
  v.x = (facing == Facing::Left ? -SPEED : SPEED);

  if (v.x == 0.0f) {
    facing = (facing == Facing::Left)
      ? Facing::Right : Facing::Left;
  }

  // Jumping
  int edgeCounter = 0;
  const int JUMP_WAIT = 3; 
  
  SDL_FRect box = bounds();

  for (const auto& p : platforms) {
    if (atEdge(box, p.bounds) == facing) {
      edgeCounter++;

      v.x *= 1.5;
      if (edgeCounter >= JUMP_WAIT) {
        if (player->y <= y) {
          v.y = -JUMP_VELOCITY;
          v.y *= 0.8;
        }
        else 
          v.y = 0;

        edgeCounter = 0;
      }
    } else edgeCounter = 0; 
  }
}
