#include <string>
#include <SDL3/SDL.h>
#include <unordered_map>
#include <memory>

class TextureCache {
public:
  ~TextureCache();

  SDL_Texture* load(const std::string& path, SDL_Renderer* renderer);
  SDL_Texture* get(const std::string& path) const;
  bool contains(const std::string& path) const;
  void remove(const std::string& path);
  void clear();

  SDL_Texture* getTexture(const std::string& id);
  bool loadTexture(const std::string& id, const std::string& path);

  size_t size() const { return cache.size(); }

private:
  struct TextureDeleter {
    void operator() (SDL_Texture* tex) const {
      if (tex) SDL_DestroyTexture(tex);
    }
  };

  using TexturePtr = std::unique_ptr<SDL_Texture, TextureDeleter>;
  std::unordered_map<std::string, TexturePtr> cache;
};
