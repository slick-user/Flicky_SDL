#include <core/renderer.hpp>

class Enemy;
class World;
class NyanNyan;
class Player;

const float SPAWN_DELAY = 1.5f;

class Spawner : public Entity {
public:

  enum class State {
    Idle,
    Spawning
  };

  SDL_Texture* texture = nullptr;
  spriteSheet* sprite = nullptr;

  State state;

  World* world = nullptr;
  Enemy* enemy = nullptr;
  Player* player = nullptr;

  float timer = 0.0f;

  Spawner(float x, float y, Renderer& r, Player* p, World* w) : Entity(x,y,32,38), player(p), world(w) {
    state = State::Idle;
    sheet = r.loadSpriteSheetJSON("Spawner", std::string(PROJECT_ROOT) + "/metadata/spawner.json");

    animator = new Animator();
    animator->setSheet(sheet);
    animator->play("idle");
  }

  void spawn();

  virtual void updateState(float dt);
  virtual void updateAnimation(float dt, State s);
  void update(float dt, const std::vector<Platform>& platforms) override;

};
