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

  // Start in menu state — level is preloaded but won't update until player presses ENTER
  state = GameState::MENU;

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

      switch (state) {

        case GameState::MENU:
          if (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER) {
            startGame();
          }
          if (e.key.key == SDLK_ESCAPE) {
            running = false;
          }
          break;

        case GameState::RUNNING:

          if (e.key.key == SDLK_R) {
            world->loadLevelViaTxt((std::string(PROJECT_ROOT) + "/levels/level1.txt"));
            // world->loadLevel((std::string(PROJECT_ROOT) + "/levels/leveltestnew.json"));
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
              edcam.x = world->camera.x + world->camera.width * 0.5f;
              edcam.y = world->camera.y + world->camera.height * 0.5f;
              world->camera.follow(edcam.x, edcam.y);
            }
          }
          break;

        case GameState::GAMEOVER:
          if (e.key.key == SDLK_R) {
            resetGame();
          }
          if (e.key.key == SDLK_ESCAPE) {
            state = GameState::MENU;
          }
          break;
      }
    }
  }
}

void Game::update(float dt) {
  switch (state) {
    case GameState::MENU:
      break;

    case GameState::RUNNING:
      if (!pause) {
        world->update(dt);
        if (world->won || world->lost) {
          state = GameState::GAMEOVER;
        }
      } else {
        edcam.handleInput(dt);
        world->camera.follow(edcam.x, edcam.y);
      }
      break;

    case GameState::GAMEOVER:
      break;
  }

  editor.update(*world, renderer, dt);
  renderGameUI();
}

void Game::render() {
  renderer.clear();

  if (state == GameState::MENU) {
    renderer.drawBlackScreen();
  } else if (!world->isRespawning()) {
    renderer.renderBackground(world->camera);
    renderer.renderWorld(*world);
  } else {
    renderer.drawBlackScreen();
  }

  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer.getSDLRenderer());

  renderer.present();
}

void Game::renderGameUI() {
  switch (state) {

    case GameState::MENU: {
      ImGui::GetForegroundDrawList()->AddRectFilled(
        ImVec2(0, 0), ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT),
        IM_COL32(0, 0, 0, 200));

      const ImGuiViewport* vp = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(ImVec2(vp->Size.x * 0.5f, vp->Size.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
      ImGui::Begin("MainMenu", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

      ImGui::SetWindowFontScale(4.0f);
      ImGui::Text("FLICKY");
      ImGui::SetWindowFontScale(1.5f);
      ImGui::Spacing();
      ImGui::Text("Press ENTER to Start");
      ImGui::Text("Press ESC to Quit");
      ImGui::SetWindowFontScale(1.0f);

      ImGui::End();
      break;
    }

    case GameState::RUNNING: {
      if (world->isRespawning()) break;

      ImGui::SetNextWindowPos(ImVec2(10, 10));
      ImGui::SetNextWindowBgAlpha(0.5f);
      ImGui::Begin("HUD", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

      ImGui::Text("Score: %d", world->getScore());
      ImGui::Text("Lives: %d", world->player ? world->player->lives : 0);
      ImGui::Text("Chicks: %d/%d", world->getChicksExited(), world->getTotalChicks());

      ImGui::End();
      break;
    }

    case GameState::GAMEOVER: {
      ImGui::GetForegroundDrawList()->AddRectFilled(
        ImVec2(0, 0), ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT),
        IM_COL32(0, 0, 0, 150));

      const ImGuiViewport* vp = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(ImVec2(vp->Size.x * 0.5f, vp->Size.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
      ImGui::SetNextWindowBgAlpha(0.8f);
      ImGui::Begin("GameOver", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

      ImGui::SetWindowFontScale(3.0f);
      if (world->won) {
        ImGui::Text("YOU WIN!");
      } else {
        ImGui::Text("GAME OVER");
      }
      ImGui::SetWindowFontScale(1.5f);
      ImGui::Spacing();
      ImGui::Text("Final Score: %d", world->getScore());
      ImGui::SetWindowFontScale(1.0f);
      ImGui::Spacing();
      ImGui::Text("Press R to Restart");
      ImGui::Text("Press ESC for Menu");

      ImGui::End();
      break;
    }
  }
}

void Game::startGame() {
  state = GameState::RUNNING;
}

void Game::resetGame() {
  world->resetIDCounters();
  world->loadLevel(std::string(PROJECT_ROOT) + "/levels/level1.json");
  state = GameState::RUNNING;
  pause = false;
}
