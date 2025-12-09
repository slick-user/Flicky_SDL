#pragma once

#include <entities/entity.hpp>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <core/camera.hpp>
#include <core/resource_loader.hpp>
#include <unordered_map>

class World;

class Renderer {
public:
  Renderer() = default;
  ~Renderer();

  // GETTER
  SDL_Renderer* getSDLRenderer() const { return renderer; }
  //TextureCache& getTextureCache() { return textureCache; }

  bool init(const char* title, int width, int height);
  void shutdown();

  void clear();
  void present();
  
  SDL_Texture* loadTexture(const std::string& id, const std::string& path);
  SDL_Texture* getTexture(const std::string& path);

  void drawTexture(SDL_Texture* tex, int x, int y, int w, int h);
  void drawTexture(SDL_Texture* tex, SDL_FRect& src, SDL_FRect& dst); 
  void drawTexture(const std::string& id, SDL_FRect& dst);
  void drawRect(const SDL_FRect& rect, SDL_Color color);

  bool loadBackground(const std::string& path);
  void renderBackground(const Camera& camera);

  void renderWorld(const World& world);

  std::unordered_map<std::string, SDL_Texture*> textures;
  //TextureCache textureCache;

private:
  SDL_Texture* backgroundTexture = nullptr;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  int screenWidth = SCREEN_WIDTH;
  int screenHeight = SCREEN_HEIGHT;
};
