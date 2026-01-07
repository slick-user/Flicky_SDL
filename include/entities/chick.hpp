#include <entities/entity.hpp>
#include <core/renderer.hpp>

class Player;

class Chick : public Entity {
public:

    enum class State {
        Idle,
        Flying,
        Following,
        
    };

    State = State::Flying;

    Player* player = nullptr;
    Entity* target = nullptr;
  
    int index = 0;

    Chick(float x, float y) : Entity(x, y, 14, 14), originX(x), originY(y) {

        state = State::Flying;

        sheet = r.loadSpriteSheetJSON("chick", std::string(PROJECT_ROOT) + "/metadata/chick.json");

        animator = new Animator();
        animator->setSheet(sheet);
        animator->play("idle");
    }

    void update(float dt, const std;:vector<Platform>& platforms) override;
    void render(Renderer& r) override;
    void onCollision() override;

    void attach(Player* p, Chick* prev, int order);

    bool isStart() const { return index == 0; }

private:
    float originX;
    float originY:
};