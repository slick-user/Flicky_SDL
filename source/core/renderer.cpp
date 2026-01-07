#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <fstream>

#include <core/renderer.hpp>

#include <core/game.hpp>
#include <core/world.hpp>
#include <entities/entity.hpp>

// Helper Functions and the like
inline bool colorWithinTolerance(
    Uint8 r, Uint8 g, Uint8 b,
    Uint8 kr, Uint8 kg, Uint8 kb,
    int tolerance
) {
    return (abs(r - kr) +
            abs(g - kg) +
            abs(b - kb)) <= tolerance;
}

void Animator::setSheet(spriteSheet* s) {
  sheet = s;
}

void Animator::play(const std::string& name, bool restart) {
  if (!sheet) return;

  auto it = sheet->animations.find(name);
    if (it == sheet->animations.end())
        return;

    if (current == &it->second && !restart)
        return;

    current = &it->second;
    frameIndex = 0;
    timer = 0.0f;


  //std::cout << "Playing: " << name << std::endl; 
}

void Animator::update(float dt) {
  if (!current) return;

  timer += dt;
  if (timer >= current->frameDuration) {
    timer -= current->frameDuration;
    frameIndex++;

    if (frameIndex >= (int)current->frames.size()) {
      frameIndex = current->loop ? 0 : current->frames.size() - 1;
    }
  }
}

int Animator::currentFrame() const {
  if (!current || current->frames.empty())
    return -1;
  return current->frames[frameIndex];
}

Renderer::~Renderer() { 
  shutdown(); 
}

bool Renderer::init(const char* title, int width, int height) {
  // Init video only: SDL_image no longer needs Init()
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL Init Error: " << SDL_GetError() << "\n";
    return false;
  }

  if (!SDL_CreateWindowAndRenderer(title, width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create Window and Renderer: %s", SDL_GetError());
    return false;
  }

  SDL_SetRenderVSync(renderer, 1);
     
  return true;
}

void Renderer::shutdown() {
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);

  renderer = nullptr;
  window = nullptr;

  SDL_Quit();
}

void Renderer::clear() {
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

SDL_Texture* Renderer::loadTexture(const std::string& id, const std::string& path) {
  SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());

  if (!tex) {
    std::cerr << "Failed to load texture " << id << ": " << SDL_GetError() << "\n";
    return nullptr;
  }
  textures[id] = tex;
  return tex;
}

SDL_Texture* Renderer::getTexture(const std::string& id) {
  auto it = textures.find(id);
  if (it == textures.end()) {
    std::cerr << "Texture not found";
    return nullptr;
  } 
  return it->second;
}

void Renderer::drawTexture(SDL_Texture* tex, SDL_FRect& src, SDL_FRect& dst) {
  SDL_RenderTexture(renderer, tex, &src, &dst);
}

void Renderer::drawTexture(const std::string& id, SDL_FRect& dst) { 
  SDL_Texture* tex = getTexture(id);
  if (tex)
    SDL_RenderTexture(renderer, tex, nullptr, &dst);
}

void Renderer::drawRect(const SDL_FRect& rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderRect(renderer, &rect);
}

void Renderer::renderWorld(const World& world) {
  // Draw Platforms
  for (auto& p : world.platforms) {
    SDL_FRect r = world.camera.apply(p.bounds);
    drawRect(r, {255,0,0,255});
  }

  // draw Entities
  for (auto& e : world.entities) {
    float entityX = e->x;
    float entityW = e->w;
    
    // Check if entity is near wrap boundaries
    bool nearLeftWrap = entityX < 100;  // Within 100 pixels of left edge
    bool nearRightWrap = entityX + entityW > SCREEN_WIDTH - 100;  // Within 100 pixels of right edge
    
    // Render main entity
    e->render(*this, world.camera);
    
    // Render wrapped copy on opposite side if near boundary
    if (nearLeftWrap) {
      // Render copy on right side
      float wrappedX = entityX + LEVEL_WIDTH;
      float originalX = e->x;
      e->x = wrappedX;
      e->render(*this, world.camera);
      e->x = originalX;  
    }
    else if (nearRightWrap) {
      // Render copy on left side
      float wrappedX = entityX - LEVEL_WIDTH;
      float originalX = e->x;
      e->x = wrappedX;
      e->render(*this, world.camera);
      e->x = originalX;  
    }
  }

}

bool Renderer::loadBackground(const std::string& path) {
  backgroundTexture = IMG_LoadTexture(renderer, path.c_str());

  if (!backgroundTexture) {
    std::cerr << "Failed to load background texture: "
              << path << "\nReason: " << SDL_GetError() << "\n";

    return false;
  }

  std::cout << "Background Texture Loaded!\n";
  return true;
} 

void Renderer::drawSprite(SDL_Texture* tex, const SDL_FRect& src, const SDL_FRect& dst, bool flipX) {
  SDL_FlipMode flip = flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  SDL_RenderTextureRotated(renderer,
                           tex, &src, 
                           &dst,
                           0.0, nullptr,
                           flip);
}

spriteSheet* Renderer::loadSpriteSheetJSON(const std::string& id, const std::string& jsonPath) {
  std::ifstream file(jsonPath);
  if (!file.is_open()) {
    std::cerr << "Failed to open " << jsonPath << "\n";
    return nullptr;
  }

  json data;
  file >> data;

  std::string imagePath = std::string(PROJECT_ROOT) + "/assets/" + data["image"].get<std::string>();

  SDL_Surface* surface = IMG_Load(imagePath.c_str());
  if (!surface) {
    std::cerr << "IMG_Load failed: " << SDL_GetError() << "\n";
    return nullptr;
  }

  // Auto-detect background color
  Uint8 kr, kg, kb;
  SDL_ReadSurfacePixel(surface, 1, 1, &kr, &kg, &kb, nullptr);

  // Convert to RGBA32
  SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
  SDL_DestroySurface(surface);
  surface = converted;

  // Fetch SDL3 pixel format details
  const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(surface->format);
  const SDL_Palette* palette = SDL_GetSurfacePalette(surface);

  int tolerance = 25;

  // Apply tolerance-based alpha masking
  Uint32* pixels = static_cast<Uint32*>(surface->pixels);
  int count = surface->w * surface->h;

  for (int i = 0; i < count; ++i) {
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixels[i], fmt, palette, &r, &g, &b, &a);

    int diff = abs(r - kr) + abs(g - kg) + abs(b - kb);

    if (diff <= tolerance) {
        pixels[i] = SDL_MapRGBA(fmt, palette, r, g, b, 0);
    }
  }

  // Convert to texture
  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);

  Uint8 a;
  SDL_ReadSurfacePixel(surface, 0, 0, nullptr, nullptr, nullptr, &a);
  SDL_DestroySurface(surface);

  if (!tex) {
    std::cerr << "CreateTexture failed: " << SDL_GetError() << "\n";
    return nullptr;
  }

  // Enable transparency blending
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

  // IMPORTANT for pixel art
  SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

  std::cout << "Color key = "
          << int(kr) << ", "
          << int(kg) << ", "
          << int(kb) << "\n";
 
  auto* sheet = new spriteSheet();
  sheet->texture = tex;

  int index = 0;

  for (auto& f : data["frames"]) {
    SDL_FRect rect {
      f["x"].get<float>(),
      f["y"].get<float>(),
      f["w"].get<float>(),
      f["h"].get<float>()
    };

    sheet->frames.push_back(rect);
    sheet->frameIndex[f["name"].get<std::string>()] = index++;
  }

  for (auto& [name, anim] : data["animations"].items()) {
    Animation a;
    float fps = anim["fps"].get<float>();
    a.frameDuration = 1.0f / fps;

    for (auto& frameName : anim["frames"]) {
        a.frames.push_back(
            sheet->frameIndex.at(frameName.get<std::string>())
        );
    }

    sheet->animations[name] = a;
  }

  spriteSheets[id] = sheet;

  std::cout
    << "[SpriteSheet] Loaded '" << id << "'\n"
    << "  Texture: " << imagePath << "\n"
    << "  Frames: " << sheet->frames.size() << "\n"
    << "  Animations: " << sheet->animations.size() << "\n";

  return sheet;
}

// INTERESTING TILE REPEATING BEHAVIOUR
void Renderer::renderBackground(const Camera& camera)
{
    float texW, texH;
    SDL_GetTextureSize(backgroundTexture, &texW, &texH);

    for (int x = 0; x < SCREEN_WIDTH; x += texW) {
        for (int y = 0; y < SCREEN_HEIGHT; y += texH) {

          float scaleX = (float)SCREEN_WIDTH / texW;
          float scaleY = (float)SCREEN_HEIGHT / texH;
          float scale = std::max(scaleX, scaleY);

            SDL_FRect dst{
                -(float)camera.x,
                -(float)camera.y,
                texW * scale,
                texH * scale     
            };

            SDL_RenderTexture(renderer, backgroundTexture, nullptr, &dst);
        }
    }
}


void Renderer::drawBlackScreen() {
  SDL_FRect screen = {0, 0, (float)screenWidth, (float)screenHeight};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderRect(renderer, &screen);
}

