#include <core/game.hpp>
#include <entities/entity.hpp>

Game::Game() : running(true) {}
Game::~Game() {}

bool Game::init() {
  if (!renderer.init("Flicky", SCREEN_WIDTH, SCREEN_HEIGHT)) {
    std::cout << "Failed to render";
    return false;
  }

  camera = new Camera(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);

  world = new World(renderer.getSDLRenderer());
  world->loadLevel((std::string(PROJECT_ROOT) + "/levels/level1.txt").c_str());
  
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
  delete camera;
  delete world;
  renderer.shutdown(); 

  //delete backgroundImage;
 
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

  float PlayerX = world->player->p.x;
  float PlayerY = world->player->p.y;

  camera->follow(PlayerX, PlayerY);

}

void Game::render() {
  renderer.clear();
  world->render(renderer.getSDLRenderer(), *camera);
  renderer.present();
}

