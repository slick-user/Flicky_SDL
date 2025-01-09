#include <stdio.h>
#include <stdbool.h>

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

// The class stuff we will modify later
#include "game.cpp" 

Game game;

/*
// CONSTANT PARAMETERS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// ANIMATION RELATED MACROS
#define PLAYER_IMAGE_HEIGHT 18
#define PLAYER_IMAGE_WIDTH 9

const int PLAYER_HEIGHT = PLAYER_IMAGE_HEIGHT * 2;
const int PLAYER_WIDTH = PLAYER_IMAGE_WIDTH * 2;

const int NUM_FRAMES = 2;

int currentFrame = 0;
Uint32 lastFrameTime = 0;
const Uint32 FRAME_DELAY = 100; // Milliseconds per frame

// PLAYER PROPERTIES
#define GRAVITY 10
#define SPEED 5

int initSDL();
void init_platforms(SDL_Rect Platform[]);
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
SDL_Texture *background_texture = NULL;

*/

int main(int argc, char* args[]) {

//                                          ====INITIALIZATION====
  game.init();

  bool d = true;
  while (d) {
    game.run();
    d = false;
  }
  /*
  if (initSDL() != 0)
    return 1; 

  // "Entity" Setup
  SDL_Rect Player = (SDL_Rect) {20, 200, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect Flicky_Image = (SDL_Rect) {1, 1, PLAYER_IMAGE_WIDTH, PLAYER_IMAGE_HEIGHT};
  SDL_Rect Background_Image = (SDL_Rect) {0, 0, 255, 223};

  SDL_Rect Platform[11];
  init_platforms(Platform);

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
 
    bool on_platform = false;
    
    // Collision Check
    for (int i=0; i<11; i++) {
      if ( (Player.x > Platform[i].x && Player.x < Platform[i].w) && (Player.y > Platform[i].y && Player.y < Platform[i].h) ) {
        on_platform = true;
        break;
      }
    }
    if (on_platform == true) {
      can_jump = true;
    }
    else if (Player.y < SCREEN_HEIGHT - 70 && on_platform == false) {
      Player.y += GRAVITY - 9;
    }
    else {
      can_jump = true;
    }
      
    
    //printf(" Player : %d", Player.y);
    updateAnimation(currentTime);

    // This is to set background color and to reset the render
    SDL_RenderClear(renderer);

    // Render the Background
    SDL_RenderCopy(renderer, background_texture, &Background_Image, NULL);
     
    if (moved == 0)
      SDL_RenderCopy(renderer, player_texture, &Flicky_Image, &Player);       // IDLE 
    else if (moved == 1)
      renderFrame(renderer, player_texture, RUNNING, 0, currentFrame, Player.x, Player.y);
    else if (moved == 2) 
      renderFrame(renderer, player_texture, RUNNING, 1, currentFrame, Player.x, Player.y);

    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 0);

    // This is used to render the image (and overwrite)``
    //scc(SDL_RenderCopy(renderer, player_texture, &Flicky_Image, &Player));
    SDL_RenderPresent(renderer);
    
    moved = false;

    SDL_Delay(1000/60);
  }
   
  kill();

*/

  return 0;
}

/*
//          INITIALIZATION FUNCTION

int initSDL() {

  SDL_InitSubSystem(SDL_INIT_VIDEO);

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    printf("Error Initializing SDL\n");
    system("pause");
    return 1;
  }

  // the defined macros are used for screen width and screen height
  window = SDL_CreateWindow("Flicky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

  IMG_Init(IMG_INIT_JPG);

  //SDL_Surface *image = scp(IMG_Load("./assets/4k-Abstract-Red-Alien-World-4K-Wallpaper.jpg"));
  //SDL_Surface *image = scp(IMG_Load("./assets/Arcade - Flicky - Flicky.png"));
  //player_texture = scp(SDL_CreateTextureFromSurface(renderer, image));
  //SDL_FreeSurface(image);

  background_texture = load_texture("./assets/Sega Genesis 32X - Flicky - Area 1.png", renderer);
  player_texture = load_texture("./assets/Arcade - Flicky - Flicky.png", renderer);

  if (!player_texture) {
    printf("Unable to load image %s\n", IMG_GetError());
    return 1;
  }

  return 0;
}

void init_platforms(SDL_Rect Platform[]) {
  // PLATFORMS
  Platform[0].x = -20;
  Platform[0].y = 342;
  Platform[0].w = 115;
  Platform[0].h = 345;

  Platform[1].x = -20;
  Platform[1].y = 240;
  Platform[1].w = 115;
  Platform[1].h = 245;

  Platform[2].x = -20;
  Platform[2].y = 135;
  Platform[2].w = 115;
  Platform[2].h = 140;

  Platform[3].x = -20;
  Platform[3].y = 32;
  Platform[3].w = 115;
  Platform[3].h = 37;

  // MIDDLE PLATFORMS
  Platform[4].x = 185;
  Platform[4].y = 290;
  Platform[4].w = 435;
  Platform[4].h = 295;
  
  Platform[5].x = 185;
  Platform[5].y = 188;
  Platform[5].w = 435;
  Platform[5].h = 193;

  Platform[6].x = 185;
  Platform[6].y = 85;
  Platform[6].w = 435;
  Platform[6].h = 90;

} */

//          ANIMATION FUNCTIONS

/*
SDL_Texture* load_texture(const char* texture_file, SDL_Renderer* renderer) {
  SDL_Surface *image = IMG_Load(texture_file); 
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 136, 136)); // Magenta as transparent 
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  return texture; 
} 

void renderFrame(SDL_Renderer* renderer, SDL_Texture* spriteSheet, int animation, bool flipped, int frame, int x, int y) {

  SDL_Rect srcRect = { frame * PLAYER_IMAGE_WIDTH + 1, (animation * PLAYER_IMAGE_HEIGHT)*3, PLAYER_IMAGE_WIDTH, PLAYER_IMAGE_HEIGHT}; 

  if (frame == 1) {
    srcRect.x = 18; 
  }
  
  SDL_Rect destRect = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
  
  if (!flipped)
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, 0, NULL, SDL_FLIP_NONE);
  else if (flipped)
    SDL_RenderCopyEx(renderer, spriteSheet, &srcRect, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void updateAnimation(Uint32 currentTime) {
  if (currentTime > lastFrameTime + FRAME_DELAY) {
    currentFrame = (currentFrame + 1) % NUM_FRAMES;
    lastFrameTime = currentTime;
    //printf("%d", currentFrame);
  }
}

//          KILL FUNCTION

*/

/*

void kill(){
  SDL_DestroyTexture(player_texture);
  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  IMG_Quit();
  
  SDL_Quit();
}

*/
