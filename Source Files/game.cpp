#include "../Header Files/game.h"

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
  Background_Image->w = 510;
  Background_Image->h = 446;
  
  return 0;
}
 
void Game::run() {

  int offset = 0; // X Offset to move the camera

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
    
    Player.handleInput(state);

    // CAMERA MOVEMENT 

    // Screen wrapping
                            // from end (right) back to start (left) screen wrapping
    if (Player.P.x > SCREEN_WIDTH) {
      for (int i=0; i<15; i++) {
        std::cout << Platform[i].x << std::endl;
      }
      camera.x = 0;
      Player.P.x = 0;
      initPlatforms();
      offset = 0;
    }
                            // from start (left) to end (right) screen wrapping
    else if (Player.P.x < 0) {
      camera.x = -640;
      Player.P.x = 640;
      initPlatforms();
      for (int i=0; i<15; i++) {
        Platform[i].x -= 640;
      }
      offset = 192;
    }
    
    //Camera scrolling
    if (Player.P.x - offset > SCREEN_WIDTH - 300 && Player.vel[0] > 0 && (camera.x > -SCREEN_WIDTH))  {
      offset += Player.vel[0];
      camera.x -= 10;
      Player.P.x -= 1;

      for (int i=0; i<15; i++) {
        Platform[i].x -= 10;      
      }
    }
    else if (Player.P.x - offset < 300 && Player.vel[0] < 0 && (camera.x < 0) ) {
      offset += Player.vel[0];
      camera.x += 10;
      Player.P.x += 1;
      // moves the platforms relative to the camera as is required
      for (int i=0; i<15; i++) {
        Platform[i].x += 10;      
      }
    }

    Player.checkCollisions(Platform);

    img.updateAnimation(); 

    // This is to to reset the render
    SDL_RenderClear(renderer);

    // Render the Background
    SDL_RenderCopy(renderer, background_texture, Background_Image, &camera);

    // Rendering the Platforms
    for (int i=0; i<15; i++) {
      SDL_RenderCopy(renderer, background_texture, Background_Image, &Platform[i]);
    }


    Player.update();

    Player.render(renderer, Flicky_Image, img.currentFrame);

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

