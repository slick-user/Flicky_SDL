#pragma once

#include <string>

#include <imgui.h>
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "./spriteEditor.hpp"
#include <entities/entity.hpp>

class World;
class Renderer;

class Editor {
public:

  void init(SDL_Renderer* renderer, SDL_Window* window);
  void frameUpdate(); // Is for certain frame updates
  void render();
  void shutdown();
  
  void toggle();
  void update(World& world, Renderer& r, float dt);  // Is for Game to update

  void renderPreviews(World* world);

  void saveLevel(const std::string& path, World* world) const;

private:  
  bool open = false;
  std::string selectedType = "Chick";
  EntityID selectedEntityID = INVALID_ID;

  char backgroundPath[512] = "";
  std::string currentBackgroundPath = "";
  void drawBackgroundLoader(Renderer* r);

  char levelPath[260] = "";
  std::string currentLevelPath;


  void drawEntityPallete(Renderer* r, World* world, ImGuiIO* io);
  
  SpriteEditor spriteEditor;
  bool spriteEditorOpen = false;
  bool backgroundLoaderOpen = false;
  bool entityPalleteOpen = false;
  
  float worldX;
  float worldY;

  bool showGrid = false; // Don't have a button for show or hide grid
  
  // Game References
  SDL_Renderer* renderer;
  SDL_Window* window;
};

