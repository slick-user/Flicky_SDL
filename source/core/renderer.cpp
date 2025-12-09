#include <core/renderer.hpp>

#include <core/world.hpp>
#include <entities/entity.hpp>

Renderer::~Renderer() { 
  shutdown(); 
}

bool Renderer::init(const char* title, int width, int height) {
   // Init video only: SDL_image no longer needs Init()
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << "\n";
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

  //return textureCache.load(path, renderer);
}

SDL_Texture* Renderer::getTexture(const std::string& id) {
  auto it = textures.find(id);
  if (it == textures.end()) return nullptr;
  return it->second;
  //textureCache.get(path);
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
  if (world.backgroundTexture) {
    SDL_FRect src = world.backgroundSrc;
    SDL_FRect dst = world.camera.apply(world.backgroundDst);
    drawTexture(world.backgroundTexture, src, dst);
  }

  // Draw Platforms
  for (auto& p : world.platforms) {
    SDL_FRect r = world.camera.apply(p);
    drawRect(r, {255,0,0,255});
  }

  // draw Entities
  for (auto& e : world.entities) {
    e->render(*this, world.camera);
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
/*
void Renderer::renderBackground(const Camera& camera) {
  if (!backgroundTexture)
    return;

  float texW = 0, texH = 0;
  SDL_GetTextureSize(backgroundTexture, &texW, &texH);


  float scaleX = (float)SCREEN_WIDTH / texW;
  float scaleY = (float)SCREEN_HEIGHT / texH;
  float scale = std::max(scaleX, scaleY);

  SDL_FRect dst {
    -(float)camera.x,
    -(float)camera.y,
    texW * scale,
    texH * scale
  };

  SDL_RenderTexture(renderer, backgroundTexture, nullptr, &dst);
}
*/

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

