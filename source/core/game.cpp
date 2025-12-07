#include "../../include/core/game.h"

Game::Game() {

  //                                          ====INITIALIZATION====
  if (initSDL() != 0)
    return;  

  initPlatforms();

  // Player Setup
  player = new Player(renderer, 20, 200, 18, 36);
 
  // Enemy Setup
  enemy = new Enemy(player, renderer, 0, 100, 18, 36);

  projectile = new Projectile(player, renderer, 0, 200, 12, 24); 

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
        case SDL_EVENT_QUIT:
          gameLoop = 0;
          break;
      }
    }

    const bool *state = SDL_GetKeyboardState(NULL);
   
    // =======================================UPDATE===========================

    updateCamera();

    player->update(platform, platformCount);
    
    enemy->update(platform, platformCount);

    projectile->update(platform, platformCount);

    player->handleInput(state);

    enemy->move();
  
    img.updateAnimation(); 

    // ======================================RENDER============================

    // This is to to reset the render
    SDL_RenderClear(renderer);

    // Render the Background
    for (int offset = -1; offset <= 1; offset++) {
      SDL_Rect dest = {-camera.x + offset * LEVEL_WIDTH, -camera.y, LEVEL_WIDTH, LEVEL_HEIGHT};

SDL_FRect bgSrc = {
    (float)backgroundImage->x,
    (float)backgroundImage->y,
    (float)backgroundImage->w,
    (float)backgroundImage->h
};

SDL_FRect bgDst = {
    (float)dest.x,
    (float)dest.y,
    (float)dest.w,
    (float)dest.h
};

SDL_RenderTexture(renderer, backgroundTexture, &bgSrc, &bgDst);

    }

    // Rendering the Platforms
    for (const auto& platform:platform) {
      for (int offset = - 1; offset <= 1; offset++) {
        SDL_Rect platformDest = {platform.x - camera.x, platform.y - camera.y, platform.w, platform.h};


SDL_FRect pSrc = {
    (float)backgroundImage->x,
    (float)backgroundImage->y,
    (float)backgroundImage->w,
    (float)backgroundImage->h
};

SDL_FRect pDst = {
    (float)platformDest.x,
    (float)platformDest.y,
    (float)platformDest.w,
    (float)platformDest.h
};

SDL_RenderTexture(renderer, backgroundTexture, &pSrc, &pDst);
     }
    } 

    player->render(renderer, img.currentFrame, camera.x, camera.y);    
   
    // Enemy Rendering
    enemy->render(renderer, img.currentFrame, camera.x, camera.y);

    projectile->render(renderer, img.currentFrame, camera.x, camera.y);
  
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
  window = SDL_CreateWindow("Flicky", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  renderer = SDL_CreateRenderer(window, NULL);
  SDL_SetRenderVSync(renderer, 1);

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

  read >> spawnerCount;

  for (int i=0; i<spawnerCount; i++) {
    read >> spawner[i].x >> spawner[i].y >> spawner[i].w >> spawner[i].h;
  }

  read.close();

}

void Game::updateCamera() {

  // Camera Following
  int targetCameraX = player->p.x - SCREEN_WIDTH/2;

  camera.x = targetCameraX;
  //                          SMOOTH CAMERA
  //int cameraDiff = targetCameraX - camera.x;
  //camera.x += cameraDiff * 0.1f;

  // Handling Screen Wrapping 
  if (targetCameraX < -LEFT_WRAP_BOUNDS) {
    // Moving left past boundary
    player->p.x += LEVEL_WIDTH;
    targetCameraX += LEVEL_WIDTH;
    enemy->p.x += LEVEL_WIDTH;
  } else if (targetCameraX >= RIGHT_WRAP_BOUNDS) {
    // Moving right past boundary  
    player->p.x -= LEVEL_WIDTH;
    targetCameraX -= LEVEL_WIDTH;
    enemy->p.x -= LEVEL_WIDTH;
  }

  //std::cout << "Camera.x : " << camera.x << "    Player.x : " << player->p.x << std::endl;
}

Game::~Game() {
  SDL_DestroyRenderer(renderer);

  delete backgroundImage;

  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;
 
  SDL_Quit();
}

void Game::shutdown() {
  return;
}

bool Game::init() {
  return true;
}
