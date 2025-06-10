#include "../Header Files/game.h"

Game::Game() {

  //                                          ====INITIALIZATION====
  if (initSDL() != 0)
    return; 

  // "Entity" Setup
  
  initPlatforms();

  // Player Setup
  player = new Player(renderer, 20, 200, 18, 36);

  flickyImage->x = 1;
  flickyImage->y = 1;
  flickyImage->w = PLAYER_IMAGE_WIDTH;
  flickyImage->h = PLAYER_IMAGE_HEIGHT;

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

    player->checkCollision(platform, platformCount);

    img.updateAnimation(); 

    // This is to to reset the render
    SDL_RenderClear(renderer);

    // Render the Background
    SDL_RenderCopy(renderer, backgroundTexture, backgroundImage, &camera);

    // Rendering the Platforms
    for (int i=0; i<platformCount; i++) {
      SDL_RenderCopy(renderer, backgroundTexture, backgroundImage, &platform[i]);
    }


    player->update();

    player->render(renderer, flickyImage, img.currentFrame);

    // Enemy Rendering
    //Enemy.render(renderer, flickyImage, img.currentFrame);

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
      
  int offset = 0; // X Offset to move the camera 

  // Screen wrapping
                            // from end (right) back to start (left) screen wrapping
  if (player->p.x > SCREEN_WIDTH) {
    for (int i=0; i<platformCount; i++) {
       //std::cout << platform[i].x << std::endl;
    }
    camera.x = 0;
    player->p.x = 0;
    initPlatforms();
    offset = 0;
  }
                            // from start (left) to end (right) screen wrapping
  else if (player->p.x < 0) {
    camera.x = -SCREEN_WRAP_OFFSET;
    player->p.x = SCREEN_WRAP_OFFSET;
    initPlatforms();
    for (int i=0; i<platformCount; i++) {
      platform[i].x -= SCREEN_WRAP_OFFSET;
    }
    offset = 192;
  }
    
  //Camera scrolling
  if (player->p.x - offset > SCREEN_WIDTH - CAMERA_OFFSET_THRESHOLD && player->vel[0] > 0 && (camera.x > -SCREEN_WIDTH))  {
    offset += player->vel[0];
    camera.x -= 10;
    player->p.x -= 1;

    for (int i=0; i<platformCount; i++) {
      platform[i].x -= 10;      
    }
  }
  else if (player->p.x - offset < CAMERA_OFFSET_THRESHOLD && player->vel[0] < 0 && (camera.x < 0) ) {
    offset += player->vel[0];
    camera.x += 10;
    player->p.x += 1;
    // moves the platforms relative to the camera as is required
    for (int i=0; i<platformCount; i++) {
      platform[i].x += 10;      
    }
  }
}

void Game::kill() {
  SDL_DestroyRenderer(renderer);

  delete flickyImage;
  delete backgroundImage;

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  IMG_Quit();
  
  SDL_Quit();
}

