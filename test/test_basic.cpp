#include <catch2/catch_test_macros.hpp>

#include <core/renderer.hpp>
#include <core/game.hpp>
#include <core/world.hpp>
#include <entities/entity.hpp>
#include <core/renderer.hpp>
#include <entities/nyannyan.hpp>
#include <entities/spawner.hpp>
#include <core/world.hpp>
#include <entities/player.hpp>
#include <iostream>

#include <SDL3/SDL.h>

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

TEST_CASE("NyanNyan sprite sheet and animations", "[entity]") {
    Renderer r;
    r.init("Test", -1, 0);
    Player player(0,0,r);  // simple player
    NyanNyan nyan(0,0,r,&player);

    SECTION("Sheet is loaded") {
        REQUIRE(nyan.sheet != nullptr);
    }

    SECTION("Animator exists") {
        REQUIRE(nyan.animator != nullptr);
    }

    SECTION("Required animations exist") {
        std::vector<std::string> keys = {"idle","walk","jump","fall"};

        for (auto& key : keys) {
            bool found = false;
            if (nyan.sheet && nyan.sheet->animations.find(key) != nyan.sheet->animations.end()) {
                found = true;
            }
            REQUIRE(found);  // FAIL if animation is missing
        }
    }

    SECTION("Can play animations safely") {
        std::vector<std::string> keys = {"idle","walk","jump","fall"};
        for (auto& key : keys) {
            // Catch unordered_map::at throws
            REQUIRE_NOTHROW(nyan.animator->play(key));
        }
    }
}

TEST_CASE("Spawner sprite sheet and animations", "[entity]") {
    Renderer r;
    r.init("Test", 1, 1);
    World world(20, 20, r);
    Player player(0,0,r);
    Spawner sp(0,0,r,&player,&world);

    SECTION("Sheet is loaded") {
      INFO("Sheet pointer is: " << sp.sheet);
      REQUIRE(sp.sheet != nullptr);
    }

    SECTION("Animator exists") {
      INFO("Animator is: " << sp.animator);
      REQUIRE(sp.animator != nullptr);
    }

    SECTION("Required animations exist") {
      std::vector<std::string> keys = {"idle","spawning"};
      for (auto& key : keys) {
        CAPTURE(key);            // 1️⃣ check animation exists
        auto it = sp.sheet->animations.find(key);
        REQUIRE(it != sp.sheet->animations.end());
          
        REQUIRE_NOTHROW(sp.animator->play(key));
      }
    }
}
