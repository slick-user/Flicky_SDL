// SDL Libraries
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

// Game
#include <core/game.hpp>
#include <entities/entity.hpp>

// Editor
#include <tools/editor.hpp>
#include "tools/editorCam.cpp"

// Level Editor Dependent Libraries
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

Game::Game() : running(true) {}
Game::~Game() {}

bool Game::init() {

  // Initialize SDL and Graphics with our Renderer
  if (!renderer.init("Flicky", SCREEN_WIDTH, SCREEN_HEIGHT)) return false;

  /* I wanted to put the ImGui stuff directly in the Editor Files but I that would take
     me more time for no reason having this directly in game, set a bool to decide if it gets used or not*/
  // ==== IMGUI SETUP / EDITOR SETUP ====
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(renderer.getWindow(), renderer.getSDLRenderer());
  ImGui_ImplSDLRenderer3_Init(renderer.getSDLRenderer());

  // ==== GAME SETUP ====

  // Background Rendering
  std::string BGpath = std::string(PROJECT_ROOT) + "/assets/Sega Genesis 32X - Flicky - Area 1.png";
  renderer.loadBackground(BGpath);

  world = new World(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);
  //world->loadLevel(std::string(PROJECT_ROOT) + "/levels/level1.txt");
  world->loadLevel((std::string(PROJECT_ROOT) + "/levels/leveltestnew.json"));

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

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    update(dt);
    
    ImGui::Render();
    render();
  }
}

void Game::shutdown() {
  delete world;

  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

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
        world->loadLevel((std::string(PROJECT_ROOT) + "/levels/level1 - Copy.txt"));
      }

      if (e.key.key == SDLK_F1) {
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

  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer.getSDLRenderer());

  renderer.present();
}

