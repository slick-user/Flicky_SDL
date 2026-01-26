#pragma once

#include <string>

class World;
class Renderer;
class Entity;

class Editor {
public:

  void toggle();
  void update(World& world, Renderer& r);

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

  std::string openFileDialog();
  std::string saveFileDialog();
};
