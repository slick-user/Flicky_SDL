#include <stdio.h>

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

void scc(int code);
void *scp(void *ptr);

int init();
void kill();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL; 

int main(int argc, char* args[]) {

//                                          ====INITIALIZATION====
  if (init() != 0)
    return 1; 

  // "Entity" Setup
  SDL_Rect Player = (SDL_Rect) {0, 40, 40, 40};


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
      Player.y -= 10;
    }

    if (state[SDL_SCANCODE_DOWN]) {
      Player.y += 10;
    }

    if (state[SDL_SCANCODE_RIGHT]) {
      Player.x += 10;
    }

    if (state[SDL_SCANCODE_LEFT]) {
      Player.x -= 10;
    }
   
    
    // This is to set background color and to reset the render
    scc(SDL_RenderClear(renderer));

    scc(SDL_SetRenderDrawColor(renderer, 0, 0, 20, 0));

    // This is used to render the image (and overwrite)``
    scc(SDL_RenderCopy(renderer, texture, NULL, &Player));
    SDL_RenderPresent(renderer);

    SDL_Delay(10);
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

  IMG_Init(IMG_INIT_JPG);

  SDL_Surface *image = scp(IMG_Load("./assets/4k-Abstract-Red-Alien-World-4K-Wallpaper.jpg"));
  texture = scp(SDL_CreateTextureFromSurface(renderer, image));
  SDL_FreeSurface(image);

  if (!texture) {
    printf("Unable to load image %s\n", IMG_GetError());
    return 1;
  }

  return 0;
}

void kill(){
  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  IMG_Quit();
  
  SDL_Quit();
}

