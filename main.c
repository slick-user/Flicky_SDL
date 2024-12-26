#include <float.h>
#include <stdio.h>

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_video.h"

void scc(int code);
void *scp(void *ptr);

int init();
void kill();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(int argc, char* args[]) {

  if (init() != 0)
    return 1;
 
  SDL_Event event;

  unsigned int gameLoop = 1;
  while (gameLoop) {
    
    while (SDL_PollEvent(&event)) {

      switch(event.type) {
        case SDL_QUIT:
          gameLoop = 0;
          break;
      }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP]) {
      // whatever the heck will be the movement
    }

    if (state[SDL_SCANCODE_DOWN]) {

    }

    if (state[SDL_SCANCODE_RIGHT]) {

    }

    if (state[SDL_SCANCODE_LEFT]) {

    }

  }
   
  kill();

  return 0;
}

void *scp(void *ptr) {
  if (ptr == NULL){
    fprintf(stderr, "SDL_ERROR: %s\n", SDL_GetError());
    exit(1);
  }
  return ptr;
}

void scc(int code) {
  if (code < 0) {
    fprintf(stderr, "SDL_ERROR: %s\n", SDL_GetError());
    exit(1);
  } 
}

int init(){

  scc(SDL_InitSubSystem(SDL_INIT_VIDEO));

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    printf("Error Initializing SDL\n");
    system("pause");
    return 1;
  }

  window = scp(SDL_CreateWindow("Flicky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0));

  renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

  return 0;
}


void kill(){
  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  SDL_Quit();
}
