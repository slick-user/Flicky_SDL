#include "editor.hpp"

#include "editor.hpp"
#include <imgui.h>

#include <core/world.hpp>
#include <entities/entity.hpp>

float cw = 140.0f;
float ch = 8.0f;

void Editor::toggle() {
    open = !open;
}

void Editor::update(World& world, Renderer& r) {
    if (!open) return;

    ImGui::Begin("Level Editor");

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
    }
    if (ImGui::Button("Increase Width")) cw -= 10.0f;
    if (ImGui::Button("Decrease Width")) cw -= 10.0f;

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

    ImGui::End();
}
