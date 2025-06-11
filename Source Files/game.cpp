#include "../Header Files/game.h"

Game::Game() {

  //                                          ====INITIALIZATION====
  if (initSDL() != 0)
    return; 

  // "Entity" Setup
  
  initPlatforms();

  // Player Setup
  player = new Player(renderer, 20, 200, 18, 36);
 
  // Enemy Setup

  // Background setup
  backgroundImage->x = 0;
  backgroundImage->y = 0;
  backgroundImage->w = 510;
  backgroundImage->h = 446;

  run();

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
    
    player->moved = 0; 
    
    player->handleInput(state);

    updateCamera();

    player->update();

    player->checkCollision(platform, platformCount);

    img.updateAnimation(); 

    // This is to to reset the render
    SDL_RenderClear(renderer);

    // Render the Background
    SDL_Rect backgroundDest = {-camera.x, -camera.y, LEVEL_WIDTH, LEVEL_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, backgroundImage, &backgroundDest);

    // Rendering the Platforms
    for (const auto& platform:platform) {
      SDL_Rect platformDest = {platform.x - camera.x, platform.y - camera.y, platform.w, platform.h};
      SDL_RenderCopy(renderer, backgroundTexture, backgroundImage, &platformDest);
    }

    player->render(renderer, img.currentFrame, camera.x, camera.y);

    // Enemy Rendering
    //Enemy.render(renderer, flickyImage, img.currentFrame);

    // This is used to render the image (and overwrite)
    SDL_RenderPresent(renderer);

    SDL_Delay(1000/60);

  }

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

  backgroundTexture = img.load_texture("./assets/Sega Genesis 32X - Flicky - Area 1.png", renderer);

  return 0;

}

void Game::initPlatforms() {
 
  std::ifstream read;
  read.open("./levels/level1.txt");

  read >> platformCount;
  platform.resize(platformCount);

  for (int i=0; i<platformCount; i++) {
    read >> platform[i].x >> platform[i].y >> platform[i].w >> platform[i].h;
  }

  read.close();

}

void Game::updateCamera() {

  // Camera Following
  int targetCameraX = player->p.x - SCREEN_WIDTH/2;

  // Clamping to screen Boundaries
  camera.x = std::max(0, std::min(targetCameraX, LEVEL_WIDTH - SCREEN_WIDTH));

  // Handling Screen Wrapping
  if (player->p.x > LEVEL_WIDTH) {
    player->p.x = 0;
    camera.x = 0;
  } else if (player->p.x < 0) {
    player->p.x = LEVEL_WIDTH;
    camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
  }
}

Game::~Game() {
  SDL_DestroyRenderer(renderer);

  delete backgroundImage;

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  IMG_Quit();
  
  SDL_Quit();
}

