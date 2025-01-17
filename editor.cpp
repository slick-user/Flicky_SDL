#include "editor.h"

int Editor::init() {
  
  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    system("pause");
    return 1;
  }

  window = SDL_CreateWindow("Flicky Level Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  IMG_Init(IMG_INIT_JPG);

  background_texture = img.load_texture("./assets/Sega Genesis 32X - Flicky - Area 1.png", renderer); 

  Background_Image->x = 0;
  Background_Image->y = 0;
  Background_Image->w = 255;
  Background_Image->h = 223;

  for (int i=0; i<no_of_platforms; i++) {
    Platform[i].h = 17;
    Platform[i].w = 100; 
  }

  return 0;
}

void Editor::run() {

  SDL_ShowCursor(SDL_DISABLE);
    
  SDL_Event event;

  bool editorLoop = true;

  while (editorLoop) {

    while (SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          editorLoop = false;
          break;
 
        case SDL_MOUSEMOTION:
          x = event.motion.x;
          y = event.motion.y;
          break;  

        case SDL_MOUSEBUTTONUP:
          if (event.button.button == SDL_BUTTON_LEFT) {
            placeTile(x, y, no_of_platforms);
          }

          else if (event.button.button == SDL_BUTTON_RIGHT) {
            deleteTile(x, y, no_of_platforms);
          }
          break;
      
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_SPACE) {
            saveFile("./levels/level1.txt");  
          }

          break;
          sad
      }
    }

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, background_texture, Background_Image, NULL);

    Platform[no_of_platforms - 1].x = x-50;
    Platform[no_of_platforms - 1].y = y-10;

    for (int i=0; i<no_of_platforms - 1; i++) {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(renderer, &Platform[i]);  
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &Platform[no_of_platforms - 1]);  

    SDL_RenderPresent(renderer);

    SDL_Delay(1000/60);

  }

  kill();
}

void Editor::placeTile(int pos_x, int pos_y, int &no_of_platforms) {

  no_of_platforms += 1;
  SDL_Rect *newPlatform = new SDL_Rect[no_of_platforms];

  for (int i=0; i<no_of_platforms; i++) {
    if (i < no_of_platforms - 1) { 
      newPlatform[i].x = Platform[i].x;
      newPlatform[i].y = Platform[i].y;
      newPlatform[i].h = Platform[i].h; //17;
      newPlatform[i].w = Platform[i].w; //100;
    }
    else {
      //newPlatform[i].h = Platform[i].h + 18;
      //newPlatform[i].w = Platform[i].w + 50;
      newPlatform[i].h = 17;
      newPlatform[i].w = 100;
    }
  }

  delete[] Platform;

  Platform = newPlatform;

}

void Editor::deleteTile(int pos_x, int pos_y, int &no_of_platforms) {

  for (int i=0; i<no_of_platforms - 1; i++) {
    if (pos_x > Platform[i].x && pos_x < Platform[i].x + Platform[i].w && pos_y > Platform[i].y && pos_y < Platform[i].y + Platform[i].h) {
      Platform[i].x = -500;
      Platform[i].y = -500;
      Platform[i].w = 0;
      Platform[i].h = 0;
    }
  }

}

void Editor::saveFile(const char *filename) {

  std::cout << filename << " FILE SAVED!"; 

  std::ofstream write;
  
  write.open(filename);

  write << no_of_platforms << std::endl;
 
  for (int i=0; i<no_of_platforms; i++) {
    write << Platform[i].x << "    " << Platform[i].y << "    " << Platform[i].w << "    " << Platform[i].h << std::endl; 
  }

  write.close();

}

void Editor::kill() {
  SDL_DestroyTexture(background_texture);
  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  renderer = nullptr;
  window = nullptr;

  delete[] Platform;

  IMG_Quit();

  SDL_Quit();
}

