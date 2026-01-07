#include <entities/chick.hpp>

void Chick::update(float dt, const std::vector<Platform>* platforms) {
    Entity::update(dt, platforms);
    updateState(dt);
    updateAnimation(dt, state);
}

void updateState(float dt) {

}

void render(Renderer& r, const Camera& camera) {
    SDL_FRect dst = cam.apply({std::floor(x), std::floor(y), w, h});
    r.drawRect(dst, {255, 255, 0, 255});
}