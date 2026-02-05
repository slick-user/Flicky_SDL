#include "editor.hpp"

#include "editorUtils.hpp"

#include <core/game.hpp>
#include <imgui.h>

#include <core/world.hpp>
#include <entities/entity.hpp>

#include <fstream>

float cw = 140.0f;  // c width
float ch = 8.0f; // c height


void getEntityPreviewSize(const std::string& type, float& w, float& h) {
    if (type == "Player") {
        w = 18.0f;
        h = 36.0f;
    } else if (type == "Spawner") {
        w = 32.0f;
        h = 38.0f;
    } else if (type == "Chick") {
        w = 24.0f;
        h = 24.0f;
    } else if (type == "Entrance") {
        w = 32.0f;
        h = 32.0f;
    } else if (type == "Projectile") {
        w = 16.0f;
        h = 16.0f;
    } else {    // Default size
        w = 32.0f;
        h = 32.0f;
    }
}

void Editor::renderPreviews(World* world) {
    if (!open) return;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    if (!drawList) return;

    ImVec2 mousePos = ImGui::GetMousePos();

    float worldX = mousePos.x + world->camera.x;
    float worldY = mousePos.y + world->camera.y;

    float screenX = worldX - world->camera.x;
    float screenY = worldY - world->camera.y;

    SDL_FRect previewPlatform = {worldX, worldY, cw, ch};
    SDL_FRect screenPlatform = world->camera.apply(previewPlatform);

    ImU32 platformColor = IM_COL32(255, 0, 0, 180);  // Red with transparency
    drawList->AddRect(
        ImVec2(screenPlatform.x, screenPlatform.y),
        ImVec2(screenPlatform.x + screenPlatform.w, screenPlatform.y + screenPlatform.h),
        platformColor,
        0.0f, 0, 2.0f  // Thickness 2
    );

        // Preview Entity (when entity type is selected)
        if (!selectedType.empty()) {
            float ew, eh;
            getEntityPreviewSize(selectedType, ew, eh);
            
            SDL_FRect previewEntity = {worldX, worldY, ew, eh};
            SDL_FRect screenEntity = world->camera.apply(previewEntity);
            
            // Draw entity preview (green outline)
            ImU32 entityColor = IM_COL32(0, 255, 0, 180);  // Green with transparency
            drawList->AddRect(
                ImVec2(screenEntity.x, screenEntity.y),
                ImVec2(screenEntity.x + screenEntity.w, screenEntity.y + screenEntity.h),
                entityColor,
                0.0f, 0, 2.0f  // Thickness 2
            );
            
            // Draw entity type label
            ImVec2 labelPos(screenEntity.x, screenEntity.y - 20);
            drawList->AddText(labelPos, IM_COL32(255, 255, 255, 255), selectedType.c_str());
        }
        
        // Draw all existing platforms (for reference)
        for (const auto& p : world->platforms) {
            SDL_FRect screenRect = world->camera.apply(p.bounds);
            drawList->AddRect(
                ImVec2(screenRect.x, screenRect.y),
                ImVec2(screenRect.x + screenRect.w, screenRect.y + screenRect.h),
                IM_COL32(255, 0, 0, 255),  // Solid red
                0.0f, 0, 1.0f
            );
        }
        
        // Draw all existing entities (for reference)
        for (const auto& e : world->getEntities()) {
            if (!e) continue;
            SDL_FRect entityBounds = e->bounds();
            SDL_FRect screenRect = world->camera.apply(entityBounds);
            
            // Different colors for different entity types
            ImU32 color = IM_COL32(0, 255, 0, 255);  // Default green
            std::string type = e->getEntityType();
            if (type == "Player") color = IM_COL32(0, 0, 255, 255);      // Blue
            else if (type == "Enemy") color = IM_COL32(255, 0, 0, 255); // Red
            else if (type == "Chick") color = IM_COL32(255, 255, 0, 255); // Yellow
            else if (type == "Entrance") color = IM_COL32(255, 0, 255, 255); // Magenta
            else if (type == "Projectile") color = IM_COL32(0, 255, 255, 255); // Cyan
            
            drawList->AddRect(
                ImVec2(screenRect.x, screenRect.y),
                ImVec2(screenRect.x + screenRect.w, screenRect.y + screenRect.h),
                color,
                0.0f, 0, 1.0f
            );
            
            // Highlight selected entity
            if (selectedEntity == e.get()) {
                drawList->AddRect(
                    ImVec2(screenRect.x - 2, screenRect.y - 2),
                    ImVec2(screenRect.x + screenRect.w + 2, screenRect.y + screenRect.h + 2),
                    IM_COL32(255, 255, 0, 255),  // Yellow highlight
                    0.0f, 0, 3.0f
                );
            }
        }
}

void Editor::toggle() {
    open = !open;
}

void Editor::update(World& world, Renderer& r, float dt) {
  if (!open) return;

    ImGui::Begin("Level Editor");
  

  if (ImGui::Button("Open Sprite Editor")) {
    spriteEditorOpen = !spriteEditorOpen;
    printf("Sprite editor toggled: %d\n", spriteEditorOpen);
  }

  if (spriteEditorOpen)
    spriteEditor.drawSpriteEditor(r, dt);

  // ---- Background Texture ----
  ImGui::Separator();
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
    std::string defaultPath = std::string(PROJECT_ROOT) + "/assets/Sega Genesis 32X - Flicky - Area 1.png";
    strncpy_s(backgroundPath, defaultPath.c_str(), sizeof(backgroundPath) - 1);
    backgroundPath[sizeof(backgroundPath) - 1] = '\0';
  }
        
  if (ImGui::Button("Browse...")) {
    std::string selectedFile = openFileDialog();
    if (!selectedFile.empty()) {
      strncpy_s(backgroundPath, selectedFile.c_str(), sizeof(backgroundPath) - 1);
      backgroundPath[sizeof(backgroundPath) - 1] = '\0';
    }
  }

  // Load background button
  if (ImGui::Button("Load Background")) {
    if (strlen(backgroundPath) > 0) {
      if (r.loadBackground(backgroundPath)) {
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
  if (ImGui::BeginPopupModal("Background Loaded", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Background loaded successfully!");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
  
  if (ImGui::BeginPopupModal("Background Load Fail", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Failed to load background!");
    ImGui::Text("Check the path and file format.");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
  ImGui::Separator();

  // ---- Entity Palette ----
  ImGui::Text("Spawn Entity");
  const char* types[] = {
    "Player",
    "Spawner",
    //"Nyannyan"   seems to crash my game,
    "Chick",
    "Entrance",
    "Projectile"
  };

  for (auto t : types) {
    if (ImGui::Selectable(t, selectedType == t))
      selectedType = t;
  }

  if (ImGui::Button("Spawn at Camera")) {
    float cx = world.camera.x + world.camera.width * 0.5f;
    float cy = world.camera.y + world.camera.height * 0.5f;
    world.spawnEntity(selectedType, cx, cy);
  }
  //                                                                  IsMouseClicked(2) is onMiddleClick 
  if (ImGui::Button("Spawn at Cursor") || ImGui::IsMouseClicked(2)) {
    float cx = ImGui::GetMousePos().x + world.camera.x; 
    float cy = ImGui::GetMousePos().y + world.camera.y; 
    world.spawnEntity(selectedType, cx, cy);
  }
  if (ImGui::IsMouseClicked(1)) {     // For Platform Creation
    float cx = ImGui::GetMousePos().x + world.camera.x; 
    float cy = ImGui::GetMousePos().y + world.camera.y; 
    world.platformCount++;
    world.platforms.emplace_back();
    world.platforms.back().bounds = {cx, cy, cw, ch};

    // Rendering the possible entity that could be here?

    // Rendering the platform that could be here?
  }
  if (ImGui::Button("Increase Width")) cw += 10.0f;
  if (ImGui::Button("Decrease Width")) cw -= 10.0f;

  ImGui::Text("Platform Width: %.1f", cw);
  ImGui::Text("Platform Height: %.1f", ch);

  ImGui::Separator();

  // ---- Entity List ----
  ImGui::Text("Entities");
  for (auto& e : world.getEntities()) {
    if (!e) continue;
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
  if (ImGui::Button("Save Level")) {
    if (strlen(levelPath) > 0) {
      saveLevel(levelPath, &world);
      currentLevelPath = levelPath;
      ImGui::OpenPopup("Level Saved");
    }
  }

  if (!currentLevelPath.empty()) {
    ImGui::Text("Current: %s", currentLevelPath.c_str());
  }

  if (ImGui::BeginPopupModal("Level Saved", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Level saved successfully!");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::End();

  renderPreviews(&world);
}

void Editor::saveLevel(const std::string& originalPath, World* world) const {
   std::string path = originalPath;

    if (path.size() < 5 || path.substr(path.size() - 5) != ".json") {
        path += ".json";
    } 

  json j;

  j["meta"] = {
    {"width", LEVEL_WIDTH},
    {"width", LEVEL_HEIGHT}
  };
  
  // Background
  if(!currentBackgroundPath.empty()) {
    j["meta"]["background"] = currentBackgroundPath;
  }

  // Player
  if (world->player) {
    j["player"] = {
      {"x", world->player->x},
      {"y", world->player->y}
    };
  }
  
  // Platforms
  j["platforms"] = json::array();
  for (const auto& p : world->platforms) {
    j["platforms"].push_back({
      {"x", p.bounds.x},
      {"y", p.bounds.y},
      {"w", p.bounds.w},
      {"h", p.bounds.h}
    }); 
  }

  // Entities
  j["entities"] = json::array();
  for (const auto& e : world->entities) {
    if (!e) continue;

    std::string type = e->getEntityType();
    if (type == "Player") continue;

    j["entities"].push_back({
      {"type", type},
      {"x", e->x},
      {"y", e->y}
    });
  }

  std::ofstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to save the level" << path << std::endl;
    return;
  }

  file << j.dump(4);
  std::cout << "Level saved to " << path << std::endl;
}

