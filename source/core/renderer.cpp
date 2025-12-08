#include <core/renderer.hpp>

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

SDL_Texture* Renderer::loadTexture(const std::string& path) {
  SDL_Surface* surface = IMG_Load(path.c_str());
  if (!surface) {
    std::cerr << "IMG Load Error: " << SDL_GetError() << "\n";
    return nullptr;
  }

  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  return tex;
}

void Renderer::drawTexture(SDL_Texture* tex, int x, int y, int w, int h) {
    SDL_FRect dst = { (float)x, (float)y, (float)w, (float)h };
    SDL_RenderTexture(renderer, tex, nullptr, &dst);
}
