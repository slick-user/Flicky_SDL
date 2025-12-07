#pragma once

#include "entities/entity.h"
#include "../util/headers.h"

class Player;

class Projectile : public Entity {  

  protected:   

    // 0 for pickable, 1 for picked, 2 for thrown
    int state = 0;
    int dir = 0;

  public:
    
    Player* player;

    Projectile(Player* player, SDL_Renderer* renderer, int x=20, int y=200, int w=18, int h=36); 
   
    void setState(int i);
    void setDir(int i);

    virtual void checkCollision(const std::vector<SDL_Rect>& platform, const int platformCount);

    virtual void update(const std::vector<SDL_Rect>& platform, const int platformCount);
};
