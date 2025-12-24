/*
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

TEST_CASE("Animator advances frames correctly", "[animator]") {
  Animation walk;
  walk.frames = {0 ,1 ,2};
  walk.frameDuration = 0.1f;
  walk.loop = true;

  spriteSheet sheet;
  sheet.animations["walk"] = walk;

  Animator a;
  a.setSheet(&sheet);
  a.play("walk");

  a.update(0.1f);
  REQUIRE(a.currentFrame() == 1);

  a.update(0.1f);
  REQUIRE(a.currentFrame() == 2);

  a.update(0.1f);
  REQUIRE(a.currentFrame() == 0);
}
*/
