#include <stdio.h>
#include <stdbool.h>

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

// CONSTANT PARAMETERS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// ANIMATION RELATED MACROS
#define PLAYER_IMAGE_HEIGHT 18
#define PLAYER_IMAGE_WIDTH 9

#define PLAYER_HEIGHT 57
#define PLAYER_WIDTH 33

const int NUM_FRAMES = 2;

int currentFrame = 0;
Uint32 lastFrameTime = 0;
const Uint32 FRAME_DELAY = 100; // Milliseconds per frame

// PLAYER PROPERTIES
#define GRAVITY 10
#define SPEED 10

void scc(int code);
void *scp(void *ptr);

int init();
void kill();

enum {
  IDLE,
  RUNNING,
};

// ANIMATION Functions
SDL_Texture* load_texture(const char* texture, SDL_Renderer* renderer);
void renderFrame(SDL_Renderer* renderer, SDL_Texture* spriteSheet, int animation, bool flipped, int frame, int x, int y);
void updateAnimation(Uint32 currentTime);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *player_texture = NULL; 

int main(int argc, char* args[]) {

//                                          ====INITIALIZATION====
  if (init() != 0)
    return 1; 

  // "Entity" Setup
  SDL_Rect Player = (SDL_Rect) {0, 0, 3 * PLAYER_IMAGE_WIDTH, 3 * PLAYER_IMAGE_HEIGHT};
  SDL_Rect Flicky_Image = (SDL_Rect) {1, 1, PLAYER_IMAGE_WIDTH, PLAYER_IMAGE_HEIGHT};
  
  bool can_jump = true;

  SDL_Event event;

  bool gameLoop = true;
  while (gameLoop) {
    Uint32 currentTime = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {

      switch(event.type) {
        case SDL_QUIT:
          gameLoop = 0;
          break;
      }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // MOVEMENT
    
    int moved = 0;
    
    if (state[SDL_SCANCODE_UP] && can_jump) {
      printf("jumped!");
      Player.y -= 100;
      can_jump = false;
    }

    if (state[SDL_SCANCODE_RIGHT]) {
      Player.x += SPEED;
      moved = 1;
    }
    if (state[SDL_SCANCODE_LEFT]) {
      Player.x -= SPEED;
      moved = 2;
    }
  
    if (Player.y < SCREEN_HEIGHT - (PLAYER_HEIGHT + 4)) {
      Player.y += GRAVITY;
    }
    else {
      can_jump = true;
    }

    updateAnimation(currentTime);

    // This is to set background color and to reset the render
    scc(SDL_RenderClear(renderer));
 
    SDL_RenderCopy(renderer, player_texture, &Flicky_Image, &Player);       // IDLE 

    if (moved == 1)
      renderFrame(renderer, player_texture, RUNNING, 0, currentFrame, Player.x, Player.y);
    else if (moved == 2) {
      renderFrame(renderer, player_texture, RUNNING, 1, currentFrame, Player.x, Player.y);
    }

    scc(SDL_SetRenderDrawColor(renderer, 0, 0, 20, 0));

    // This is used to render the image (and overwrite)``
    //scc(SDL_RenderCopy(renderer, player_texture, &Flicky_Image, &Player));
    SDL_RenderPresent(renderer);
    
    moved = false;

    SDL_Delay(1000/60);
  }
   
  kill();

  return 0;
}

// CHECKING FOR ERRORS IN INITIALIZATION
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

//          INITIALIZATION FUNCTION

int init() {

  scc(SDL_InitSubSystem(SDL_INIT_VIDEO));

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    printf("Error Initializing SDL\n");
    system("pause");
    return 1;
  }

  // the defined macros are used for screen width and screen height
  window = scp(SDL_CreateWindow("Flicky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0));

  renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ));

  IMG_Init(IMG_INIT_JPG);

  //SDL_Surface *image = scp(IMG_Load("./assets/4k-Abstract-Red-Alien-World-4K-Wallpaper.jpg"));
  //SDL_Surface *image = scp(IMG_Load("./assets/Arcade - Flicky - Flicky.png"));
  //player_texture = scp(SDL_CreateTextureFromSurface(renderer, image));
  //SDL_FreeSurface(image);

  player_texture = load_texture("./assets/Arcade - Flicky - Flicky.png", renderer);

  if (!player_texture) {
    printf("Unable to load image %s\n", IMG_GetError());
    return 1;
  }

  return 0;
}

//          ANIMATION FUNCTIONS

SDL_Texture* load_texture(const char* texture_file, SDL_Renderer* renderer) {
  SDL_Surface *image = scp(IMG_Load(texture_file)); 
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 255, 0, 255)); // Magenta as transparent 
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  return texture; 
} 

void renderFrame(SDL_Renderer* renderer, SDL_Texture* spriteSheet, int animation, bool flipped, int frame, int x, int y) {

  SDL_Rect srcRect = { frame * PLAYER_IMAGE_WIDTH, (animation * PLAYER_IMAGE_HEIGHT)*3, PLAYER_IMAGE_WIDTH, PLAYER_IMAGE_HEIGHT}; 

  if (frame == 1) {
    srcRect.x = 18; 
  }
  
  SDL_Rect destRect = {x, y, 3 * PLAYER_IMAGE_WIDTH, 3 * PLAYER_IMAGE_HEIGHT};
  
  if (!flipped)
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, 0, NULL, 0);
  else if (flipped)
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, 0, NULL, 1);
}

void updateAnimation(Uint32 currentTime) {
  if (currentTime > lastFrameTime + FRAME_DELAY) {
    currentFrame = (currentFrame + 1) % NUM_FRAMES;
    lastFrameTime = currentTime;
    //printf("%d", currentFrame);
  }
}

//          KILL FUNCTION

void kill(){
  SDL_DestroyTexture(player_texture);
  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  IMG_Quit();
  
  SDL_Quit();
}

