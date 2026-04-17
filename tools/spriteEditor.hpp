#pragma once
#include <imgui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <vector>
#include <string>

struct EditorFrame {
    std::string name;
    SDL_FRect rect;
};

struct EditorAnimation {
    std::string name;
    float fps = 8.0f;
    std::vector<int> frames; // indices into EditorFrame
};

struct EditorSpriteSheet {
    std::string sheetName;
    std::string imagePath;

    SDL_Texture* texture = nullptr;
    int texW = 0;
    int texH = 0;

    std::vector<EditorFrame> frames;
    std::vector<EditorAnimation> animations;
};

// Forward declaration
class Renderer;

class SpriteEditor {
public:
    void drawSpriteEditor(Renderer& r, float dt);
    void saveSpriteSheetJSON(const std::string& path);

private:
  EditorSpriteSheet sheet;

  // Frame selection
  SDL_FRect selection{};
  ImVec2 selectStart{};
  ImVec2 selectEnd{};
  char frameName[64]{};

  // Animations
  char animName[64]{};
  int selectedAnim = -1;

  // Preview
  float previewTimer = 0.0f;
  int previewFrame = 0;

  void handleFrameSelection();
  void drawPreview(float dt);

  // Windows
  void drawControlsWindow(Renderer& r);
  void drawImageWindow();
  void drawAnimationWindow();
  void drawPreviewWindow(float dt);

  enum class SelectState {
    Idle,
    Selecting,
    Selected
  };

  SelectState selectState = SelectState::Idle;

  float zoom = 1.0f;
  float zoomMin = 0.25f;
  float zoomMax = 4.0f;

  int gridSize = 16;
  bool gridSnap = false;
  bool showGrid = false;
};
