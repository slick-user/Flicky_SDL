#include "include/core/game.hpp"

int main(int argc, char* args[]) {

  Game game;
  if (!game.init()) {
    std::cout << "Game did not initialize properly!\n";
    return -1;
  }

  game.run();
  game.shutdown();

  return 0;
}
