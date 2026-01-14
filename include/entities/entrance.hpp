#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Player;
class Chick;

class Entrance : public Entity {
public:

  enum class State {
    Idle,
    Win
  };

  SDL_Texture* texture = nullptr;
  spriteSheet* sprite = nullptr;

  State state;

  World* world = nullptr;

  Entrance(float x, float y, Renderer& r, World* w) : Entity(x, y, 32, 32), world(w) {
    state = State::Idle;
    sheet = r.loadSpriteSheetJSON("Spawner", std::string(PROJECT_ROOT) + "/metadata/entrance.json");

    animator = new Animator();
    animator->setSheet(sheet);
    animator->play("idle");
  }

  virtual void updateState(float dt);
  virtual void updateAnimation(float dt, State s);

  void update(float dt, const std::vector<Platform>& platforms) override;

  const char* getEntityType() const override { return "Entrance"; }

  void onCollision(Entity* e) override;
  
};
