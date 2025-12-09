#include <entities/player.hpp>
#include <core/camera.hpp>
#include <core/renderer.hpp>

void Player::update(float dt, const std::vector<SDL_FRect>& platforms) {
    // TODO: Input, gravity, jumping, collisions, etc.
}

void Player::render(Renderer& r, const Camera& cam) {
  SDL_FRect dst { x, y, w, h };  
  dst = cam.apply(dst);
  if (texture)
    r.drawTexture(textureId, dst);
  else
    r.drawRect(dst, {0, 255, 0, 0});
}

