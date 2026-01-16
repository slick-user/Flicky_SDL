#pragma once

#include <string>
#include <unordered_map>

#include <entities/entity.hpp>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <core/camera.hpp>
#include <libs/json.hpp>

using json = nlohmann::json;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 520;

class World;

// Entity Animations
struct Animation {
  std::vector<int> frames;
  float frameDuration;
  bool loop = true;
};

struct spriteSheet {
  SDL_Texture* texture = nullptr;
  std::vector<SDL_FRect> frames;
  std::unordered_map<std::string, int> frameIndex;
  
  std::unordered_map<std::string, Animation> animations;
};

//    == RESPONISBLE FOR THE ANIMATIONS PLAYING ==
class Animator {
public:
  void setSheet(spriteSheet* sheet);
  void update(float dt);
  void play(const std::string& name, bool restart=false);
  int currentFrame() const;

private:
  float timer = 0.0f;
  int frameIndex = 0;
  
  spriteSheet* sheet = nullptr;
  Animation* current = nullptr;
};

// The Renderer
class Renderer {
public:
  Renderer() = default;
  ~Renderer();

  // GETTER
  SDL_Renderer* getSDLRenderer() const { return renderer; }
  SDL_Window* getWindow() const { return window; }

  bool init(const char* title, int width, int height);
  void shutdown();

  void clear();
  void present();
  
  SDL_Texture* loadTexture(const std::string& id, const std::string& path);
  SDL_Texture* getTexture(const std::string& path);

  void drawTexture(SDL_Texture* tex, SDL_FRect& src, SDL_FRect& dst); 
  void drawTexture(const std::string& id, SDL_FRect& dst);

  void drawRect(const SDL_FRect& rect, SDL_Color color);

  bool loadBackground(const std::string& path);
  void renderBackground(const Camera& camera);
  
  spriteSheet* loadSpriteSheetJSON (const std::string& id, const std::string& jsonPath);

  void drawSprite(SDL_Texture* tex, const SDL_FRect& src, const SDL_FRect& dst, bool flipX);

  void renderWorld(const World& world);

  std::unordered_map<std::string, SDL_Texture*> textures;
  std::unordered_map<std::string, spriteSheet*> spriteSheets;

  // Special Case Methods
  void drawBlackScreen();

private:
  SDL_Texture* backgroundTexture = nullptr;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  int screenWidth = SCREEN_WIDTH;
  int screenHeight = SCREEN_HEIGHT;
};
