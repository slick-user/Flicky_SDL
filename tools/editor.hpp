#pragma once

#include <iostream>
#include <string>

class World;
class Renderer;
class Entity;

class Editor {
public:
  
  void toggle();
  void update(World& world, Renderer& r);

private:
  bool open = false;
  std::string selectedType = "Chick";
  Entity* selectedEntity = nullptr;
};
