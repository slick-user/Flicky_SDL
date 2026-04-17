#pragma once

#include <string>

#include <imgui.h>
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "./spriteEditor.hpp"

class World;
class Renderer;
class Entity;

// TODO make difference between update functions clear

class Editor {
public:

  void init(SDL_Renderer* renderer, SDL_Window* window);
  // Is for Game to update
  void update(); // Is for certain frame updates
  void render();
  void shutdown();
  
  void toggle();
  void update(World& world, Renderer& r, float dt);

  void renderPreviews(World* world);

  void saveLevel(const std::string& path, World* world) const;

private:  
  bool open = false;
  std::string selectedType = "Chick";
  Entity* selectedEntity = nullptr;

  char backgroundPath[512] = "";
  std::string currentBackgroundPath = "";
  void drawBackgroundLoader(Renderer* r);

  char levelPath[260] = "";
  std::string currentLevelPath;

  SpriteEditor spriteEditor;
  bool spriteEditorOpen = false;
  bool backgroundLoaderOpen = false;
  
  float worldX;
  float worldY;

  // Game References
  SDL_Renderer* renderer;
  SDL_Window* window;
};

