#include <entities/enemy.hpp>
#include <entities/entity.hpp>

class NyanNyan : public Enemy {
public:

  NyanNyan(float x, float y, Renderer& r, Player* p) : Enemy(x,y,r,p, EntityType::NyanNyan) { 
    state = State::Idle;

    facing = Facing::Left;

    sheet = r.loadSpriteSheetJSON("Enemy", std::string(PROJECT_ROOT) + "/metadata/enemy.json");

    animator = new Animator();
    animator->setSheet(sheet);
    animator->play("idle");
  }
 
  //virtual void update(float dt, const std::vector<Platform>& platforms) override;

  void ai(float dt, const std::vector<Platform>& platforms) override;

  EntityType getEntityTypeEnum() const override { return EntityType::NyanNyan; }
  const char* getEntityType() const override { return "NyanNyan"; }

};
