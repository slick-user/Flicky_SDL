#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <core/game.hpp>
#include <entities/entity.hpp>

Game::Game() : running(true) {}
Game::~Game() {}

bool Game::init() {

  if (!renderer.init("Flicky", SCREEN_WIDTH, SCREEN_HEIGHT)) return false;

  // Background Rendering
  std::string BGpath = std::string(PROJECT_ROOT) + "/assets/Sega Genesis 32X - Flicky - Area 1.png";
  renderer.loadBackground(BGpath);

  world = new World(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);
  world->loadLevel(std::string(PROJECT_ROOT) + "/levels/level1.txt");
  //world->loadLevel((std::string(PROJECT_ROOT) + "/levels/level1 - Copy.txt"));

  return true;
} 

void Game::run() {

  Uint64 last = SDL_GetPerformanceCounter();
  const double freq = (double)SDL_GetPerformanceFrequency();

  while (running) {
    Uint64 now = SDL_GetPerformanceCounter();
    float dt = float((now - last) / freq);
    last = now;

    processEvents();
    update(dt);
    render();
  }
}

void Game::shutdown() {
  delete world;
  renderer.shutdown();  

  SDL_Quit();
}

void Game::processEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_QUIT) {
      running = false;
    }

    if (e.type == SDL_EVENT_KEY_DOWN) {

      if (e.key.key == SDLK_R) {
        world->loadLevel((std::string(PROJECT_ROOT) + "/levels/level1 - Copy.txt"));
      }
    }
  }

}

void Game::update(float dt) {
  world->update(dt);
}

void Game::render() {
  renderer.clear();

  if (!world->isRespawning()) {
    renderer.renderBackground(world->camera);
    renderer.renderWorld(*world);
  } else {
    renderer.drawBlackScreen();
  }

  renderer.present();
}

