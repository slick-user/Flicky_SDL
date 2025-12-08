#pragma once

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

#include <core/world.hpp>
#include <core/renderer.hpp>
#include <core/camera.hpp>
#include <entities/player.hpp>
#include <entities/enemy.hpp>
#include <entities/projectile.hpp>
#include <util/util.hpp>

//class Util;

// DIMENSIONS OF LEVEL

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 480;

const int PLAYER_HEIGHT = PLAYER_IMAGE_HEIGHT * 2;
const int PLAYER_WIDTH = PLAYER_IMAGE_WIDTH * 2;

// CAMERA CONSTANTS
const int LEFT_WRAP_BOUNDS = 320;
const int RIGHT_WRAP_BOUNDS = LEVEL_WIDTH - 320;

const int CAMERA_SCROLL_SPEED = 10;

enum {
  IDLE,
  RUNNING,
};

class Game {

  public:
    Game();
    ~Game();
    
    bool init();
    void run(); 
    void shutdown();

  private:
    void processEvents();
    void update(float dt);
    void render();

  private:
    Renderer renderer;
    World* world = nullptr;
    Camera* camera = nullptr;
    
    bool running = true;
 
    std::vector<SDL_Rect> spawner; 
    std::vector<SDL_Rect> platform;

};
