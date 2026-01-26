#include <catch2/catch_test_macros.hpp>

#include <core/renderer.hpp>
#include <core/game.hpp>
#include <core/world.hpp>
#include <entities/entity.hpp>
#include <core/renderer.hpp>
#include <entities/nyannyan.hpp>
#include <entities/spawner.hpp>
#include <entities/chick.hpp>
#include <core/world.hpp>
#include <entities/player.hpp>
#include <iostream>

#include <SDL3/SDL.h>

TEST_CASE("Renderer Initializes Correctly", "[renderer]") {
    Renderer r;
    REQUIRE(r.init("Test Init", 200, 200) == true);
    r.shutdown();
}

TEST_CASE("World initializes with empty state", "[world]") {
    Renderer r;
    REQUIRE(r.init("Test", 100, 100));

    World w(100, 100, r);

    REQUIRE(w.entities.size() > 0); // player is spawned
    REQUIRE(w.player != nullptr);
}

TEST_CASE("World update runs safely", "[world]") {
    Renderer r;
    r.init("Test", 100, 100);

    World w(100, 100, r);

    REQUIRE_NOTHROW(w.update(0.016f));
}

TEST_CASE("World respawn delay works", "[world]") {
    Renderer r;
    r.init("Test", 100, 100);

    World w(100, 100, r);

    w.startRespawnDelay();
    REQUIRE(w.isRespawning());

    w.update(10.0f); // simulate time passing
    REQUIRE_FALSE(w.isRespawning());
}

TEST_CASE("Player initializes correctly", "[player]") {
    Renderer r;
    r.init("Test", 100, 100);

    Player p(0, 0, r);

    REQUIRE(p.animator != nullptr);
    REQUIRE(p.sheet != nullptr);
    REQUIRE(p.getEntityType() == std::string("Player"));
}

TEST_CASE("Player animation state changes are safe", "[player]") {
    Renderer r;
    r.init("Test", 100, 100);

    Player p(0, 0, r);

    REQUIRE_NOTHROW(p.updateAnimation(0.016f, State::Idle));
    REQUIRE_NOTHROW(p.updateAnimation(0.016f, State::Walk));
    REQUIRE_NOTHROW(p.updateAnimation(0.016f, State::Jump));
    REQUIRE_NOTHROW(p.updateAnimation(0.016f, State::Fall));
}

TEST_CASE("NyanNyan updates safely", "[enemy]") {
    Renderer r;
    r.init("Test", 100, 100);

    Player player(0, 0, r);
    NyanNyan nyan(0, 0, r, &player);

    REQUIRE_NOTHROW(nyan.update(0.016f, {}));
}

TEST_CASE("NyanNyan requires valid player", "[enemy]") {
    Renderer r;
    r.init("Test", 100, 100);

    Player player(0, 0, r);
    NyanNyan nyan(0, 0, r, &player);

    REQUIRE(nyan.getPlayer() != nullptr);
}

TEST_CASE("Chick starts in flying state", "[chick]") {
    Renderer r;
    r.init("Test", 100, 100);

    Chick chick(0, 0, r);

    REQUIRE(chick.state == Chick::State::Flying);
}

TEST_CASE("Chick can attach to player", "[chick]") {
    Renderer r;
    r.init("Test", 100, 100);

    Player p(0, 0, r);
    Chick c(10, 10, r);

    REQUIRE_NOTHROW(c.attach(&p, nullptr, 0));
    REQUIRE(c.state == Chick::State::Following);
    REQUIRE(c.target == &p);
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

TEST_CASE("Spawner enters spawning state", "[spawner]") {
    Renderer r;
    r.init("Test", 100, 100);

    World w(100, 100, r);
    Player p(0, 0, r);

    Spawner sp(0, 0, r, &p, &w);

    sp.updateState(0.016f);
    REQUIRE(sp.state == Spawner::State::Spawning);
}

TEST_CASE("Spawner spawns enemy safely", "[spawner]") {
    Renderer r;
    r.init("Test", 100, 100);

    World w(100, 100, r);
    Player p(0, 0, r);

    Spawner sp(0, 0, r, &p, &w);

    REQUIRE_NOTHROW(sp.spawn());
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

TEST_CASE("Entity collision does not crash", "[collision]") {
    Renderer r;
    r.init("Test", 100, 100);

    World w(100, 100, r);

    REQUIRE_NOTHROW(w.checkEntityCollisions());
}

TEST_CASE("Multiple updates do not crash", "[stability]") {
    Renderer r;
    r.init("Test", 100, 100);

    World w(100, 100, r);

    for (int i = 0; i < 1000; ++i) {
        REQUIRE_NOTHROW(w.update(0.016f));
    }
}
