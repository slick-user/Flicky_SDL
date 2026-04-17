#include "spriteEditor.hpp"
#include "editorUtils.hpp"
#include <libs/json.hpp>
#include <fstream>
#include <algorithm>
#include <core/renderer.hpp> 

void SpriteEditor::drawSpriteEditor(Renderer& r, float dt) {
  drawControlsWindow(r);
  drawImageWindow();
  drawAnimationWindow();
  drawPreviewWindow(dt);
}

void SpriteEditor::drawPreview(float dt) {
    if (selectedAnim < 0 || sheet.animations[selectedAnim].frames.empty())
        return;

    auto& anim = sheet.animations[selectedAnim];
    previewTimer += dt;
    float frameTime = 1.0f / anim.fps;

    if (previewTimer >= frameTime) {
        previewTimer = 0;
        previewFrame = (previewFrame + 1) % anim.frames.size();
    }

    int frameIndex = anim.frames[previewFrame];
    SDL_FRect& src = sheet.frames[frameIndex].rect;

    ImVec2 uv0(src.x / sheet.texW, src.y / sheet.texH);
    ImVec2 uv1((src.x + src.w) / sheet.texW, (src.y + src.h) / sheet.texH);

    ImGui::Image((ImTextureID)sheet.texture, ImVec2(src.w*2, src.h*2), uv0, uv1);
}

void SpriteEditor::drawControlsWindow(Renderer& r) {
  ImGui::Begin("Sprite Sheet Controls");

  ImGui::InputText("Sheet Name", sheet.sheetName.data(), sheet.sheetName.size());

  if (ImGui::Button("Load Image")) {
    std::string path = openFileDialog();
    if (!path.empty()) {
      sheet.imagePath = path;

      SDL_Surface* surface = IMG_Load(path.c_str());
      if (surface) {
        sheet.texture = SDL_CreateTextureFromSurface(
          r.getSDLRenderer(), surface
        );
        sheet.texW = surface->w;
        sheet.texH = surface->h;
        SDL_DestroySurface(surface);
      }
    }
  }

  if (ImGui::Button("Save JSON")) {
    std::string path = saveFileDialog();
    if (!path.empty()) {
      if (!endsWith(path, ".json"))
        path += ".json";
      saveSpriteSheetJSON(path);
    }
  }

  ImGui::End();
}

void SpriteEditor::drawImageWindow() {
    ImGui::Begin("Sprite Sheet");

    if (!sheet.texture) {
        ImGui::Text("No image loaded");
        ImGui::End();
        return;
    }

    // ---- Snap To Grid ----
    ImGui::Checkbox("Snap to Grid", &gridSnap);
    ImGui::SameLine();
    ImGui::Checkbox("Show Grid", &showGrid);
    ImGui::SetNextItemWidth(100);
    ImGui::DragInt("Grid Size", &gridSize, 1, 1, 128);
    ImGui::Separator();

    // ---- Zoom handling ----
    if (ImGui::IsWindowHovered()) {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f) {
            zoom += wheel * 0.1f;
            zoom = std::clamp(zoom, zoomMin, zoomMax);
        }
    }

    ImVec2 imgSize(sheet.texW * zoom, sheet.texH * zoom);
    ImGui::Image((ImTextureID)sheet.texture, imgSize);

    ImVec2 imgPos = ImGui::GetItemRectMin();
    ImVec2 mouse = ImGui::GetMousePos();

    // Mouse position in IMAGE SPACE
    ImVec2 imgMouse{
        (mouse.x - imgPos.x) / zoom,
        (mouse.y - imgPos.y) / zoom
    };

    // Snapping
    if (gridSnap && gridSize > 0) {
      imgMouse.x = float(int(imgMouse.x / gridSize) * gridSize);
      imgMouse.y = float(int(imgMouse.y / gridSize) * gridSize);
    }

    // ---- Click-based selection state machine ----
    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {

        if (selectState == SelectState::Idle ||
            selectState == SelectState::Selected) {

            // 1st or 3rd click → start new rect
            selectStart = imgMouse;
            selectEnd   = imgMouse;
            selectState = SelectState::Selecting;
        }
        else if (selectState == SelectState::Selecting) {

            // 2nd click → finalize rect
            selectEnd = imgMouse;
            selectState = SelectState::Selected;

            selection.x = std::min(selectStart.x, selectEnd.x);
            selection.y = std::min(selectStart.y, selectEnd.y);
            selection.w = std::abs(selectEnd.x - selectStart.x);
            selection.h = std::abs(selectEnd.y - selectStart.y);
        }
    }

    ImDrawList* dl = ImGui::GetWindowDrawList();

    // ---- Draw Grid Lines ----
    if (showGrid && gridSize > 0) {
        ImU32 gridColor = IM_COL32(255, 255, 255, 40); // subtle white

        // Vertical lines
        for (int x = 0; x < sheet.texW; x += gridSize) {
            float screenX = imgPos.x + x * zoom;
            float screenY0 = imgPos.y;
            float screenY1 = imgPos.y + sheet.texH * zoom;
            dl->AddLine(ImVec2(screenX, screenY0), ImVec2(screenX, screenY1), gridColor);
        }

        // Horizontal lines
        for (int y = 0; y < sheet.texH; y += gridSize) {
            float screenY = imgPos.y + y * zoom;
            float screenX0 = imgPos.x;
            float screenX1 = imgPos.x + sheet.texW * zoom;
            dl->AddLine(ImVec2(screenX0, screenY), ImVec2(screenX1, screenY), gridColor);
        }
    }

    // ---- Live preview while selecting ----
    if (selectState == SelectState::Selecting) {
        dl->AddRect(
            ImVec2(imgPos.x + selectStart.x * zoom,
                   imgPos.y + selectStart.y * zoom),
            ImVec2(imgPos.x + imgMouse.x * zoom,
                   imgPos.y + imgMouse.y * zoom),
            IM_COL32(255, 255, 0, 255),
            0.0f, 0, 2.0f
        );
    }

    // ---- Finalized selection ----
    if (selectState == SelectState::Selected &&
        selection.w > 0 && selection.h > 0) {

        dl->AddRect(
            ImVec2(imgPos.x + selection.x * zoom,
                   imgPos.y + selection.y * zoom),
            ImVec2(imgPos.x + (selection.x + selection.w) * zoom,
                   imgPos.y + (selection.y + selection.h) * zoom),
            IM_COL32(255, 0, 0, 255),
            0.0f, 0, 2.0f
        );
    }

    ImGui::End();
}

void SpriteEditor::drawAnimationWindow() {
  ImGui::Begin("Frames & Animations");

  // Frame creation
  ImGui::InputText("Frame Name", frameName, 64);
  if (ImGui::Button("Add Frame")) {
    if (frameName[0] && selection.w > 0 && selection.h > 0) {
      sheet.frames.push_back({ frameName, selection });
      frameName[0] = '\0';
      selection = {};
    }
  }

  ImGui::Separator();
  ImGui::Text("Frames:");

  for (auto& f : sheet.frames) {
    ImGui::BulletText(
      "%s (%.0f, %.0f, %.0f, %.0f)",
      f.name.c_str(),
      f.rect.x, f.rect.y, f.rect.w, f.rect.h
    );
  }

  ImGui::Separator();
  ImGui::InputText("Animation Name", animName, 64);

  if (ImGui::Button("Create Animation")) {
    if (animName[0]) {
      sheet.animations.push_back({ animName });
      animName[0] = '\0';
    }
  }

  for (int i = 0; i < (int)sheet.animations.size(); i++) {
    if (ImGui::Selectable(
      sheet.animations[i].name.c_str(),
      selectedAnim == i
    ))
    selectedAnim = i;
  }

  if (selectedAnim >= 0) {
    auto& anim = sheet.animations[selectedAnim];
    ImGui::DragFloat("FPS", &anim.fps, 0.1f, 1, 60);

    ImGui::Text("Add Frames:");
    for (int i = 0; i < (int)sheet.frames.size(); i++) {
      if (ImGui::SmallButton(sheet.frames[i].name.c_str()))
        anim.frames.push_back(i);
    }
  }

  ImGui::End();
}

void SpriteEditor::drawPreviewWindow(float dt) {
  ImGui::Begin("Animation Preview");

  if (selectedAnim < 0 ||
    sheet.animations[selectedAnim].frames.empty()) {
    ImGui::Text("No animation selected");
    ImGui::End();
    return;
  }

  auto& anim = sheet.animations[selectedAnim];

  previewTimer += dt;
  float frameTime = 1.0f / anim.fps;

  if (previewTimer >= frameTime) {
    previewTimer = 0.0f;
    previewFrame = (previewFrame + 1) % anim.frames.size();
  }

  int idx = anim.frames[previewFrame];
  SDL_FRect& src = sheet.frames[idx].rect;

  ImVec2 uv0(src.x / sheet.texW, src.y / sheet.texH);
  ImVec2 uv1(
    (src.x + src.w) / sheet.texW,
    (src.y + src.h) / sheet.texH
  );

  ImGui::Image(
    (ImTextureID)sheet.texture,
    ImVec2(src.w * 2, src.h * 2),
    uv0, uv1
  );

  ImGui::End();
}

void SpriteEditor::saveSpriteSheetJSON(const std::string& path) {
  json j;
  j["image"] = sheet.imagePath;

  for (auto& f : sheet.frames) {
    j["frames"].push_back({
      {"name", f.name},
      {"x", f.rect.x},
      {"y", f.rect.y},
      {"w", f.rect.w},
      {"h", f.rect.h}
    });
  }

  for (auto& a : sheet.animations) {
    json frames;
    for (int idx : a.frames)
      frames.push_back(sheet.frames[idx].name);

      j["animations"][a.name] = {
        {"frames", frames},
        {"fps", a.fps}
      };
  }

  std::ofstream(path) << j.dump(4);
}
