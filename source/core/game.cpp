// SDL Libraries
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

// Game
#include <core/game.hpp>
#include <entities/entity.hpp>

// Editor
#include <tools/editor.hpp>
#include <tools/editorCam.cpp>

Game::Game() : running(true) {}
Game::~Game() {}

bool Game::init() {
  // Initialize SDL and Graphics with our Renderer
  if (!renderer.init("Flicky", SCREEN_WIDTH, SCREEN_HEIGHT)) return false;

  editor.init(renderer.getSDLRenderer(), renderer.getWindow());
  // ==== GAME SETUP ====

  // Background Rendering
  // TODO Move to Renderer Initialization
  std::string BGpath = std::string(PROJECT_ROOT) + "/assets/Sega Genesis 32X - Flicky - Area 1.png";
  renderer.loadBackground(BGpath);

  // TODO Refactor the abstraction layers: main, game, world
  world = new World(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);
  //world->loadLevelViaTxt(std::string(PROJECT_ROOT) + "/levels/level1.txt");
  world->loadLevel((std::string(PROJECT_ROOT) + "/levels/level1.json"));

  return true;
}

void Game::run() {

  Uint64 last = SDL_GetPerformanceCounter();
  const double freq = (double)SDL_GetPerformanceFrequency();

  while (running) {
    Uint64 now = SDL_GetPerformanceCounter();
    dt = float((now - last) / freq);
    last = now;

    processEvents();

    editor.frameUpdate();
    update(dt);

    editor.render();
    render();
  }
}

void Game::shutdown() {
  delete world;

  editor.shutdown();
  renderer.shutdown();
  SDL_Quit();
}

void Game::processEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    ImGui_ImplSDL3_ProcessEvent(&e);
    if (e.type == SDL_EVENT_QUIT) {
      running = false;
    }

    if (e.type == SDL_EVENT_KEY_DOWN) {

      if (e.key.key == SDLK_R) {
        world->loadLevelViaTxt((std::string(PROJECT_ROOT) + "/levels/level1.txt"));
        // world->loadLevel((std::string(PROJECT_ROOT) + "/levels/leveltestnew.json"));
      }

      if (e.key.key == SDLK_F1) {
        // Here we could have this trigger an editor loop instead
        editor.toggle();
      }

      if (e.key.key == SDLK_F2) {
        if (pause == true) {
          pause = false;
          world->camera.follow(world->player);
        }
        else {
          pause = true;
          // Sync editor camera to current view center
          edcam.x = world->camera.x + world->camera.width * 0.5f;
          edcam.y = world->camera.y + world->camera.height * 0.5f;

          world->camera.follow(edcam.x, edcam.y);
        }
      }
    }
  }
}

void Game::update(float dt) {
  if (!pause) {
    world->update(dt);
  } else {
    edcam.handleInput(dt);
    world->camera.follow(edcam.x, edcam.y);
  }

  editor.update(*world, renderer, dt);
}

void Game::render() {
  renderer.clear();

  if (!world->isRespawning()) {
    renderer.renderBackground(world->camera);
    renderer.renderWorld(*world);
  } else {                                    // If player is hit / respawning the Screen turns to black
    renderer.drawBlackScreen();
  }

  // Could move this into proposed Editor loop
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer.getSDLRenderer());

  renderer.present();
}
