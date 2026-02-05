#pragma once

#include <string>
#include "./spriteEditor.hpp"

class World;
class Renderer;
class Entity;

class Editor {
public:

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

  char levelPath[260] = "";
  std::string currentLevelPath;

  SpriteEditor spriteEditor;
  bool spriteEditorOpen = false;
};

