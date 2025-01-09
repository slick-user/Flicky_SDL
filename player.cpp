#include <iostream>

#include "include/SDL2/SDL_rect.h"

class Entity {
   
  public:
    int x = 20; 
    int y = 200;
    int w = 18;
    int h = 36;
     
    SDL_Rect P;  // replace back with PLAYER_WIDTH AND PLAYER_HEIGHT 

    void init();
    void update();
};

void Entity::init() {
  
  P.x = x;
  P.y = y;
  P.w = w;
  P.h = h;

}

void Entity::update() {
  
  P.x = x;
  P.y = y;

}
