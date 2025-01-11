#include <stdbool.h>

// The class stuff we will modify later
#include "game.h" 

Game game;

int main(int argc, char* args[]) {

  game.init();

  bool d = true;
  while (d) {
    game.run();
    d = false;
  }
  return 0;
}
