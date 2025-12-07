#include "include/core/game.h"
//#include "Header Files/editor.h"


int main(int argc, char* args[]) {

  Game game;
  if (!game.init())
    return -1;

  game.run();
  game.shutdown();
    //Editor editor;
  return 0;
}
