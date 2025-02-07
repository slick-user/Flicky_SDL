#include <stdbool.h>

// The class stuff we will modify later
#include "Header Files/game.h"
//#include "Header Files/editor.h"

Game game;

//Editor editor;

int main(int argc, char* args[]) {

  //editor.init();
  game.init();

  bool d = true;
  while (d) {
    game.run();
    //editor.run();
    d = false;
  }
  return 0;
}
