# Compilation

g++ -o Flicky main.cpp "Source Files/game.cpp" "Source Files/player.cpp" "Source Files/util.cpp" "Source Files/entity.cpp" "Source Files/enemy.cpp" "Source Files/projectile.cpp" -O1 -Wall -I include -L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

For the Editor version:
g++ -o Flicky main.cpp "Source Files/editor.cpp" "Source Files/util.cpp" -O1 -Wall -I include -L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# Changelist

* Added Camera. we will begin development of enemies, chicks, level complete and projectiles
* Added Screen Wrapping

----------------------------------------------------------------

Ok so we were programming enemies that is the point we had reached, This is supposed to be a super modular and easy to use engine more than anything

Enemy class that then gives way to the cat enemy class. This will give us our first proper enemy and for that enemies screen wrapping.....

========================================================================================
code for a debug hitbox for the player :
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect debugBox = { player->p.x - camera.x, player->p.y - camera.y, PLAYER_WIDTH, PLAYER_HEIGHT };
  SDL_RenderDrawRect(renderer, &debugBox);

Wow this is a really broken almost garbage software and it took me way to long to write it, hopefully I write a way better version this time around

Lets start with the system, I feel like I should document this game a lot better than I have, for that I should have a place where I can document the development of this game, I know I will write a website that I will host and that is where I will write about the development of this software. I don't really feel like doing that... I can do that and do this, lets do both

It's been a while since I have revisited this thing lets fix the bad bits rewrite all the systems that need rewriting so that we don't have to worry about factorization anymore and instead we can focus on implementing features

Lets begin with our Logic separation and how we plan to sort and plan those things

already off to a bad start with the main function

# Stuff I am learning about during the process

## Version Control
I think I just found out about one of the best additions I can have to my process. Implementing and using version control. One way would be Having multiple branches 
  - `main` branch for stable builds 
  - `develop` branch for integration
  - feature/* for my systems
  - hotfix/* emergency fixes

## Architecture and Design Questions

# The Rewrite

