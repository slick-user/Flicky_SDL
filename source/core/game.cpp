#include <core/game.hpp>
#include <entities/entity.hpp>

Game::Game() : running(true) {}
Game::~Game() {}

bool Game::init() {
  if (!renderer.init("Flicky", SCREEN_WIDTH, SCREEN_HEIGHT)) return false;

  std::string BGpath = std::string(PROJECT_ROOT) + "/assets/Sega Genesis 32X - Flicky - Area 1.png";
  renderer.loadBackground(BGpath);

  world = new World(SCREEN_WIDTH, SCREEN_HEIGHT);
  world->loadLevel((std::string(PROJECT_ROOT) + "/levels/level1.txt").c_str());

  SDL_Texture* playerText = renderer.loadTexture("Player", PROJECT_ROOT "/assets/Arcade - Flicky - Flicky.png");
  Entity* player = new Player(20, 200, renderer);
  player->texture = playerText;

  world->entities.push_back(player);

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
  }
}

void Game::update(float dt) {
  world->update(dt);
}

void Game::render() {
  renderer.clear();
  renderer.renderBackground(world->camera);
  renderer.renderWorld(*world);
  renderer.present();
}

