#pragma once

#include <entities/enemy.hpp>
#include <entities/entity.hpp>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>

class Renderer {
public:
  Renderer() = default;
  ~Renderer();

  bool init(const char* title, int width, int height);
  void shutdown();

  void clear();
  void present();
  
  SDL_Texture* loadTexture(const std::string& path);

  void drawTexture(SDL_Texture* tex, int x, int y, int w, int h);

  SDL_Renderer* getSDLRenderer() const { return renderer; }
  
private:
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  int screenWidth = SCREEN_WIDTH;
  int screenHeight = SCREEN_HEIGHT;
};
