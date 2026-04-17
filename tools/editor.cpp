#include "editor.hpp"

#include "editorUtils.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include <core/game.hpp>

#include <core/world.hpp>
#include <entities/entity.hpp>

#include <fstream>

float cw = 140.0f; // c width (Platform Width)
float ch = 10.0f;   // c height (Platform Height)

int GRID_HEIGHT = 10;
int GRID_WIDTH = 10;

// Editor to Game Functions
void Editor::init(SDL_Renderer* renderer, SDL_Window* window) {
  // ==== IMGUI SETUP / EDITOR SETUP ====
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);  
}

void Editor::update() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void Editor::render() {
 ImGui::Render(); 
}

void Editor::shutdown() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

// this is still kind of really BS
void getEntityPreviewSize(const std::string &type, float &w, float &h) {
  if (type == "Player") {
    w = 18.0f; h = 36.0f;
  } else if (type == "Spawner") {
    w = 32.0f; h = 38.0f;
  } else if (type == "Chick") {
    w = 24.0f; h = 24.0f;
  } else if (type == "Entrance") {
    w = 32.0f; h = 32.0f;
  } else if (type == "Projectile") {
    w = 16.0f; h = 16.0f;
  } else { // Default size
    w = 32.0f; h = 32.0f;
  }
}

// see the new tiles placed, and where they potentially could be
void Editor::renderPreviews(World *world) {
  // Draw list is from ImGUI
  ImDrawList *drawList = ImGui::GetBackgroundDrawList();
  if (!drawList)
    return;
  
  // Set visual bounds of Platform hitbox
  SDL_FRect previewPlatform = {worldX, worldY, cw, ch};
  SDL_FRect screenPlatform = world->camera.apply(previewPlatform);

  // Preview of Platform Placement 
  ImU32 platformColor = IM_COL32(255, 0, 0, 180); // Red with transparency
  drawList->AddRect(ImVec2(screenPlatform.x, screenPlatform.y),
                    ImVec2(screenPlatform.x + screenPlatform.w,
                           screenPlatform.y + screenPlatform.h),
                    platformColor, 0.0f, 0, 2.0f // Thickness 2
  );

  // Preview Entity (when entity type is selected)
  if (!selectedType.empty()) {
    float entity_w, entity_h;
    getEntityPreviewSize(selectedType, entity_w, entity_h);

    SDL_FRect previewEntity = {worldX, worldY, entity_w, entity_h};
    SDL_FRect screenEntity = world->camera.apply(previewEntity);

    // Draw entity preview (green outline)
    ImU32 entityColor = IM_COL32(0, 255, 0, 180); // Green with transparency
    drawList->AddRect(
        ImVec2(screenEntity.x, screenEntity.y), // Think this is where we need to multiply by the Grid Size
        ImVec2(screenEntity.x + screenEntity.w,
               screenEntity.y + screenEntity.h),
        entityColor, 0.0f, 0, 2.0f // Thickness 2
    );

    // Draw entity type label
    ImVec2 labelPos(screenEntity.x, screenEntity.y - 20);
    drawList->AddText(labelPos, IM_COL32(255, 255, 255, 255),
                      selectedType.c_str());
  }

  // Draw all existing platforms (for reference)
  for (const auto &p : world->platforms) {
    SDL_FRect screenRect = world->camera.apply(p.bounds);
    drawList->AddRect(
        ImVec2(screenRect.x, screenRect.y),
        ImVec2(screenRect.x + screenRect.w, screenRect.y + screenRect.h),
        IM_COL32(255, 0, 0, 255), // Solid red
        0.0f, 0, 1.0f);
  }

  // Draw all existing entities (for reference)
  for (const auto &e : world->getEntities()) {
    if (!e)
      continue;
    SDL_FRect entityBounds = e->bounds();
    SDL_FRect screenRect = world->camera.apply(entityBounds);

    // Different color outlines for different entity types
    ImU32 color = IM_COL32(0, 255, 0, 255); // Default green
    std::string type = e->getEntityType();
    if (type == "Player")
      color = IM_COL32(0, 0, 255, 255); // Blue
    else if (type == "Enemy")
      color = IM_COL32(255, 0, 0, 255); // Red
    else if (type == "Chick")
      color = IM_COL32(255, 255, 0, 255); // Yellow
    else if (type == "Entrance")
      color = IM_COL32(255, 0, 255, 255); // Magenta
    else if (type == "Projectile")
      color = IM_COL32(0, 255, 255, 255); // Cyan

    drawList->AddRect(
        ImVec2(screenRect.x, screenRect.y),
        ImVec2(screenRect.x + screenRect.w, screenRect.y + screenRect.h), color,
        0.0f, 0, 1.0f);

    // Highlight selected
    if (selectedEntity == e.get()) {  
      drawList->AddRect(ImVec2(screenRect.x - 2, screenRect.y - 2),
                        ImVec2(screenRect.w + 2, screenRect.y + screenRect.h + 2),
                        IM_COL32(255, 255, 0, 255), // Yellow highlight
                        0.0f, 0, 3.0f);
    }
  }
}

void Editor::toggle() { open = !open; }

void Editor::drawBackgroundLoader(Renderer* r) {
  ImGui::Text("Background Texture");

  // Text input for background path
  ImGui::InputText("Path", backgroundPath, sizeof(backgroundPath));

  // Helper buttons for common paths
  if (ImGui::Button("Use Assets Folder")) {
    std::string assetsPath = std::string(PROJECT_ROOT) + "/assets/";
    strncpy_s(backgroundPath, assetsPath.c_str(), sizeof(backgroundPath) - 1);
    backgroundPath[sizeof(backgroundPath) - 1] = '\0';
  }
  ImGui::SameLine();
  if (ImGui::Button("Default Background")) {
    std::string defaultPath = std::string(PROJECT_ROOT) +
                              "/assets/Sega Genesis 32X - Flicky - Area 1.png";
    strncpy_s(backgroundPath, defaultPath.c_str(), sizeof(backgroundPath) - 1);
    backgroundPath[sizeof(backgroundPath) - 1] = '\0';
  }

  if (ImGui::Button("Browse...")) {
    std::string selectedFile = openFileDialog();
    if (!selectedFile.empty()) {
      strncpy_s(backgroundPath, selectedFile.c_str(),
                sizeof(backgroundPath) - 1);
      backgroundPath[sizeof(backgroundPath) - 1] = '\0';
    }
  }

  // Load background button
  if (ImGui::Button("Load Background")) {
    if (strlen(backgroundPath) > 0) {
      if (r->loadBackground(backgroundPath)) {
        currentBackgroundPath = backgroundPath;
        ImGui::OpenPopup("Background Loaded");
      } else {
        ImGui::OpenPopup("Background Load Failed");
      }
    }
  }

  // Show current background path
  if (!currentBackgroundPath.empty()) {
    ImGui::Text("Current: %s", currentBackgroundPath.c_str());
  }

  // Popup for success/failure
  if (ImGui::BeginPopupModal("Background Loaded", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Background loaded successfully!");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  if (ImGui::BeginPopupModal("Background Load Fail", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Failed to load background!");
    ImGui::Text("Check the path and file format.");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void Editor::update(World &world, Renderer &r, float dt) {
  if (!open)
    return;
  
  ImVec2 mousePos = ImGui::GetMousePos();
  ImGuiIO& io = ImGui::GetIO();

  // Sets Mouse Placement to be snapped to the mouse 
  /* This is done as mousePos.x while might be absolute, the camera
     (And so the background and all other assets that are also relatively
     offset with the camera do not "snap" to the mouse)*/
  worldX = mousePos.x + world.camera.x;
  worldY = mousePos.y + world.camera.y;

  worldX = (int(worldX / GRID_WIDTH)) * GRID_WIDTH;
  worldY = (int(worldY / GRID_HEIGHT)) * GRID_HEIGHT;

  ImGui::Begin("Level Editor");

  if (ImGui::Button(spriteEditorOpen ? "Close Sprite Editor" : "Open Sprite Editor")) {
    spriteEditorOpen = !spriteEditorOpen;
    printf("Sprite editor toggled: %d\n", spriteEditorOpen);
  }
  if (spriteEditorOpen)
    spriteEditor.drawSpriteEditor(r, dt);

  // ---- Background Texture ----

  if (ImGui::Button(backgroundLoaderOpen ? "Close Background Loader" : "Open Background Loader")) {
    backgroundLoaderOpen = !backgroundLoaderOpen;
    printf("Background Texture Loader toggled: %d\n", backgroundLoaderOpen);
  }
  if (backgroundLoaderOpen)
    drawBackgroundLoader(&r);
  
  // ---- Entity Palette ----
  ImGui::Text("Spawn Entity");
  const char *types[] = {"Player", "Spawner",
                         //"Nyannyan"   seems to crash my game,
                         "Chick", "Entrance", "Projectile"};

  for (auto t : types) {
    if (ImGui::Selectable(t, selectedType == t))
      selectedType = t;
  }

  if (ImGui::Button("Spawn at Camera")) {
    float cx = world.camera.x + world.camera.width * 0.5f;
    float cy = world.camera.y + world.camera.height * 0.5f;
    world.spawnEntity(selectedType, cx, cy);
  }
  //                                        IsMouseClicked(2) is MiddleClick
  if (ImGui::Button("Spawn at Cursor") || (!io.WantCaptureMouse && ImGui::IsMouseClicked(0)) || ImGui::IsKeyPressed(ImGuiKey_E)) {
    world.spawnEntity(selectedType, worldX, worldY);
  }
  if (!io.WantCaptureMouse && ImGui::IsMouseClicked(1) || ImGui::IsKeyPressed(ImGuiKey_Q)) { // For Platform Creation
    world.platformCount++;
    world.platforms.emplace_back();
    world.platforms.back().bounds = {worldX, worldY, cw, ch};
  // TODO Deletion
  /* is a bit longer due to the fact it needs to detect the entity first*/
  }
  if (ImGui::Button("Increase Width") || (ImGui::IsKeyPressed(ImGuiKey_X)))
    cw += 10.0f;
  if (ImGui::Button("Decrease Width") || (ImGui::IsKeyPressed(ImGuiKey_Z))) 
    cw -= 10.0f;

  if (!io.WantCaptureMouse) {
    if (io.MouseWheel != 0.0f) {
      if (io.KeyCtrl)
        ch += io.MouseWheel * GRID_HEIGHT;
      else
        cw += io.MouseWheel * GRID_WIDTH;

      cw = std::max(cw, 10.0f);
      ch = std::max(ch, 10.0f);
    }
  }

  ImGui::Text("Platform Width: %.1f", cw);
  ImGui::Text("Platform Height: %.1f", ch);

  ImGui::Separator();

  // ---- Entity List ----
  ImGui::Text("Entities");
  for (auto &e : world.getEntities()) {
    if (!e)
      continue;
    if (ImGui::Selectable(e->getEntityType(), selectedEntity == e.get())) {
      selectedEntity = e.get();
    }
  }

  // ---- Inspector ----
  if (selectedEntity) {
    ImGui::Separator();
    ImGui::Text("Inspector");
    float x = selectedEntity->x;
    float y = selectedEntity->y;

    if (ImGui::DragFloat("X", &x, 1.0f))
      selectedEntity->x = x;
    if (ImGui::DragFloat("Y", &y, 1.0f))
      selectedEntity->y = y;

    if (ImGui::Button("Delete")) {
      world.removeEntity(selectedEntity);
      selectedEntity = nullptr;
    }
  }

  // Levels
  ImGui::Separator();

  ImGui::Text("Level File");

  // Text input
  ImGui::InputText("Level Path", levelPath, sizeof(levelPath));

  if (ImGui::Button("Use Levels Folder")) {
    std::string levelsPath = std::string(PROJECT_ROOT) + "/levels/";
    strncpy_s(levelPath, levelsPath.c_str(), sizeof(levelPath) - 1);
    levelPath[sizeof(levelPath) - 1] = '\0';
  }

  ImGui::SameLine();
  if (ImGui::Button("Browse...")) {
    std::string path = saveFileDialog();
    if (!path.empty()) {
      strncpy_s(levelPath, path.c_str(), sizeof(levelPath) - 1);
      levelPath[sizeof(levelPath) - 1] = '\0';
    }
  }

  // Save Level button
  if ((ImGui::Button("Save Level")) || (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S))) {
    if (strlen(levelPath) > 0) {
      saveLevel(levelPath, &world);
      currentLevelPath = levelPath;
      ImGui::OpenPopup("Level Saved");
    }
  }

  if (!currentLevelPath.empty())
    ImGui::Text("Current: %s", currentLevelPath.c_str());  

  if (ImGui::BeginPopupModal("Level Saved", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Level saved successfully!");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::End();

  renderPreviews(&world);
}

void Editor::saveLevel(const std::string &originalPath, World *world) const {
  std::string path = originalPath;

  if (path.size() < 5 || path.substr(path.size() - 5) != ".json") {
    path += ".json";
  }

  json j;

  j["meta"] = {{"width", LEVEL_WIDTH}, {"height", LEVEL_HEIGHT}};

  // Background
  if (!currentBackgroundPath.empty()) {
    j["meta"]["background"] = currentBackgroundPath;
  }

  // Player
  if (world->player) {
    j["player"] = {{"x", world->player->x}, {"y", world->player->y}};
  }

  // Platforms
  j["platforms"] = json::array();
  for (const auto &p : world->platforms) {
    j["platforms"].push_back({{"x", p.bounds.x},
                              {"y", p.bounds.y},
                              {"w", p.bounds.w},
                              {"h", p.bounds.h}});
  }

  // Entities
  j["entities"] = json::array();
  for (const auto &e : world->entities) {
    if (!e)
      continue;

    std::string type = e->getEntityType();
    if (type == "Player")
      continue;

    j["entities"].push_back({{"type", type}, {"x", e->x}, {"y", e->y}});
  }

  std::ofstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to save the level" << path << std::endl;
    return;
  }

  file << j.dump(4);
  std::cout << "Level saved to " << path << std::endl;
}
