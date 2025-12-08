#pragma once

//#include <util/headers.hpp>
#include <entities/entity.hpp>

class Enemy;
class Projectile;

// PLAYER PROPERTIES
const int GRAVITY = 2;
const int SPEED = 3;

class Projectile;

class Player : public Entity{

  private:
    bool hasPot = false;

    Projectile* projectile;

    void shoot();

  public:
 
    Player(SDL_Renderer* renderer, int x=20, int y=200, int w=18, int h=36);

    // GETTTERS
    bool getPot();

    // SETTERS
    void setPot(bool i);
    void setProjectile(Projectile* projectile); 

    void handleInput(const bool *state);
    
};

