#include <core/resource_loader.hpp>
#include <SDL3_image/SDL_image.h>
#include <iostream>

TextureCache::~TextureCache() {
    clear();
}

SDL_Texture* TextureCache::load(const std::string& path, SDL_Renderer* renderer) {
    // Check cache first
    auto it = cache.find(path);
    if (it != cache.end()) {
        return it->second.get();
    }
    
    // Load new texture
    SDL_Surface* surface = IMG_Load((std::string(PROJECT_ROOT) + path).c_str());
    if (!surface) {
        std::cerr << "Failed to load texture '" << path << "': " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!texture) {
        std::cerr << "Failed to create texture from surface '" << path << "': " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    // Store in cache
    cache[path] = TexturePtr(texture);
    std::cout << "Loaded texture: " << path << "\n";
    
    return texture;
}

SDL_Texture* TextureCache::get(const std::string& path) const {
    auto it = cache.find(path);
    return (it != cache.end()) ? it->second.get() : nullptr;
}

bool TextureCache::contains(const std::string& path) const {
    return cache.find(path) != cache.end();
}

void TextureCache::remove(const std::string& path) {
    cache.erase(path);
}

void TextureCache::clear() {
    cache.clear();
}
