#pragma once

#include <stdio.h>
#include <tools/editor.hpp>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

#include <imgui.h>
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <core/world.hpp>
#include <core/renderer.hpp>
#include <core/camera.hpp>
#include <entities/player.hpp>
#include <entities/enemy.hpp>
#include <entities/projectile.hpp>

#include "../tools/editor.hpp"
#include "../tools/editorcam.cpp"

// Level Editor Dependent Libraries

//class Util;

// DIMENSIONS OF LEVEL

const int LEVEL_WIDTH = 1190;
const int LEVEL_HEIGHT = 480;

// CAMERA CONSTANTS
const int LEFT_WRAP_BOUNDS = 320;
const int RIGHT_WRAP_BOUNDS = LEVEL_WIDTH - 320;

const int CAMERA_SCROLL_SPEED = 10;

enum class GameState{
  MENU,
  RUNNING,
  GAMEOVER
};

class Game {

  public:
    Game();
    ~Game();
    
    bool init();
    void run(); 
    void shutdown();

    float dt;

  private:
    void processEvents();
    void update(float dt);
    void render();

    void resetGame();
    void startGame();
    void renderGameUI();

  private:
    Renderer renderer;
    World* world = nullptr;
    Editor editor;
    EditorCam edcam;

    bool running = true;
    bool pause = false;
 
    std::vector<SDL_Rect> spawner; 
    std::vector<SDL_Rect> platform;

    GameState state = GameState::MENU;

};
