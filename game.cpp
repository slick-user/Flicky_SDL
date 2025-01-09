#include <stdio.h>

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

#include "player.cpp"
#include "util.cpp"

// CONSTANT PARAMETERS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// ANIMATION RELATED MACROS
#define PLAYER_IMAGE_HEIGHT 18
#define PLAYER_IMAGE_WIDTH 9

const int PLAYER_HEIGHT = PLAYER_IMAGE_HEIGHT * 2;
const int PLAYER_WIDTH = PLAYER_IMAGE_WIDTH * 2;

const int NUM_FRAMES = 2;

// PLAYER PROPERTIES
#define GRAVITY 10
#define SPEED 5

enum {
  IDLE,
  RUNNING,
};

class Game {
 
  private:
     // Properties
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Entity Player;
    Util img;

    SDL_Rect Platform[11];
 
    bool initSDL();
    
    // temporary    
    SDL_Texture *player_texture = NULL; 
    SDL_Texture *background_texture = NULL;
    SDL_Rect *Flicky_Image = new SDL_Rect;
    SDL_Rect *Background_Image = new SDL_Rect;

  public:
    Game() = default;

    int init();
    void initPlatforms(SDL_Rect Platform[]);
    void run();
    void kill();
 
};

int Game::init() {
//                                          ====INITIALIZATION====
  if (initSDL() != 0)
    return 1; 

  // "Entity" Setup
 
  Player.init();

  Flicky_Image->x = 1;
  Flicky_Image->y = 1;
  Flicky_Image->w = PLAYER_IMAGE_WIDTH;
  Flicky_Image->h = PLAYER_IMAGE_HEIGHT;

  Background_Image->x = 0;
  Background_Image->y = 0;
  Background_Image->w = 255;
  Background_Image->h = 223;

  Game::initPlatforms(Platform);

  return 0;
}
 
void Game::run() {
  bool can_jump = true;

  SDL_Event event;

  bool gameLoop = true;

  while (gameLoop) {
    img.update();

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
      
    
    img.updateAnimation();

    // This is to set background color and to reset the render
    SDL_RenderClear(renderer);

    // Render the Background
    SDL_RenderCopy(renderer, background_texture, Background_Image, NULL);
     
    if (moved == 0){
      Player.update();
      SDL_RenderCopy(renderer, player_texture, Flicky_Image, &Player.P);       // IDLE 
    }
    else if (moved == 1)
      img.renderFrame(renderer, player_texture, RUNNING, 0, img.currentFrame, Player.x, Player.y);
    else if (moved == 2) 
      img.renderFrame(renderer, player_texture, RUNNING, 1, img.currentFrame, Player.x, Player.y);

    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 0);

    // This is used to render the image (and overwrite)
    SDL_RenderPresent(renderer);
    
    moved = 0;

    SDL_Delay(1000/60);

  }

  kill();
}

bool Game::initSDL() {
 
  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    printf("Error Initializing SDL\n");
    system("pause");
    return 1;
  }

  // the defined macros are used for screen width and screen height
  window = SDL_CreateWindow("Flicky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

  IMG_Init(IMG_INIT_JPG);

  background_texture = img.load_texture("./assets/Sega Genesis 32X - Flicky - Area 1.png", renderer);
  player_texture = img.load_texture("./assets/Arcade - Flicky - Flicky.png", renderer);

  if (!player_texture) {
    printf("Unable to load image %s\n", IMG_GetError());
    return 1;
  }

  return 0;

}

void Game::initPlatforms(SDL_Rect Platform[]) {
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


}

void Game::kill() {
  SDL_DestroyTexture(player_texture);
  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  IMG_Quit();
  
  SDL_Quit();
}

