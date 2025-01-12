#include "game.h"

int Game::init() {

  //                                          ====INITIALIZATION====
  if (initSDL() != 0)
    return 1; 

  // "Entity" Setup
  
  initPlatforms();

  Player.init(renderer);

  Flicky_Image->x = 1;
  Flicky_Image->y = 1;
  Flicky_Image->w = PLAYER_IMAGE_WIDTH;
  Flicky_Image->h = PLAYER_IMAGE_HEIGHT;

  Background_Image->x = 0;
  Background_Image->y = 0;
  Background_Image->w = 255;
  Background_Image->h = 223;
  
  return 0;
}
 
void Game::run() {

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
    
    Player.moved = 0; 
    
    Player.handleInput(state, Platform);
    // MOVEMENT 
  
    img.updateAnimation();
    
    // This is to to reset the render
    SDL_RenderClear(renderer);

    // Render the Background
    SDL_RenderCopy(renderer, background_texture, Background_Image, NULL);

    Player.update();

    Player.render(renderer, Flicky_Image, img.currentFrame);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 20, 0);

    // This is used to render the image (and overwrite)
    SDL_RenderPresent(renderer);

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

  return 0;

}

void Game::initPlatforms() {
 
  std::ifstream read;

  read.open("./levels/level1.txt");

  int no_of_platforms = 0;
  read >> no_of_platforms;

  Platform = new SDL_Rect[no_of_platforms];

  for (int i=0; i<no_of_platforms; i++) {
    read >> Platform[i].x >> Platform[i].y >> Platform[i].w >> Platform[i].h;
  }

  read.close();

}

void Game::kill() {
  SDL_DestroyTexture(Player.texture);
  SDL_DestroyRenderer(renderer);

  delete Flicky_Image;
  delete Background_Image;

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  IMG_Quit();
  
  SDL_Quit();
}

