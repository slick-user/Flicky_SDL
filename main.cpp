#include "include/core/game.hpp"
//#include "Header Files/editor.hpp"


int main(int argc, char* args[]) {

  Game game;
  if (!game.init()) {
    std::cout << "Game did not initialize properly!\n";
    return -1;
  }

  game.run();
  game.shutdown();
    //Editor editor;
  return 0;
}
