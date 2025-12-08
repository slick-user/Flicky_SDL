#include <catch2/catch_test_macros.hpp>

#include <core/renderer.hpp>
#include <core/game.hpp>
#include <core/world.hpp>
#include <entities/entity.hpp>

TEST_CASE("Renderer Initializes Correctly", "[renderer]") {
    Renderer r;
    REQUIRE(r.init("Test Init", 200, 200) == true);
    r.shutdown();
}

TEST_CASE("Texture loads", "[renderer]") {
    Renderer r;
    REQUIRE(r.init("Test Init", 200, 200) == true);

    SDL_Texture* tex = r.loadTexture(std::string(PROJECT_ROOT) + "/assets/Arcade - Flicky - Flicky.png");
    REQUIRE(tex != nullptr);

    r.shutdown();
}

TEST_CASE("World loads level from file", "[world]") {
  Renderer r;
  r.init("Test", 200, 200);
  World w(r.getSDLRenderer());   

  std::string path = std::string(PROJECT_ROOT) + "/levels/test_level.txt";

  REQUIRE_NOTHROW(w.loadLevel(path));

  REQUIRE(w.platforms.size() > 0);
  REQUIRE(w.platforms[0].w > 0);
}

TEST_CASE("Game initializes", "[game]") {
    Game g;

    REQUIRE(g.init() == true);

    g.shutdown();
}
