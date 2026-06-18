#include <entities/entity.hpp>
#include <core/renderer.hpp>


class Player;

class Chick : public Entity {
public:

    enum class State {
        Idle,
        Flying,
        Following, 
        Rescued
    };

    State state = State::Flying;

    Player* player = nullptr;
    Entity* target = nullptr;
  
    int index = 0;

    Chick(float x, float y, Renderer& r) : Entity(x, y, 24, 24, EntityType::Chick), originX(x), originY(y) {

        state = State::Flying;

        sheet = r.loadSpriteSheetJSON("Chick", std::string(PROJECT_ROOT) + "/metadata/chick.json");

        animator = new Animator();
        animator->setSheet(sheet);
        animator->play("idle");
    }

    void update(float dt, const std::vector<Platform>& platforms) override;
    void updateState(float dt);  
    void updateAnimation(float dt, State state);

    void onCollision(Entity* e) override;

    void attach(Player* p, Chick* prev, int order);

    bool isStart() const { return index == 0; }

    virtual const char* getEntityType() const override { return "Chick"; }
    virtual EntityType getEntityTypeEnum() const override { return EntityType::Chick; }

private:
    float originX;
    float originY;
};
