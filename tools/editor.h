#pragma once

#include "../Header Files/headers.h"

// CONSTANT PARAMETERS
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 480

class Editor {

  private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    Util img;

    int no_of_platforms = 1;
    SDL_Rect *Platform = new SDL_Rect[no_of_platforms];

    SDL_Texture *background_texture = NULL;
    SDL_Rect *Background_Image = new SDL_Rect;

    int x;
    int y; 

  public:
    Editor();

    void run();
    void kill();
  
    void placeTile(int pos_x, int pos_y, int &no_of_platforms);
    void deleteTile(int pos_x, int pos_y, int &no_of_platforms);
    
    void saveFile(const char *filename);

};

