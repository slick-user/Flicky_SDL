# Compilation

g++ -o Flicky main.cpp "Source Files/game.cpp" "Source Files/player.cpp" "Source Files/util.cpp" "Source Files/entity.cpp" "Source Files/enemy.cpp" "Source Files/projectile.cpp" -O1 -Wall -I include -L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

For the Editor version:
g++ -o Flicky main.cpp "Source Files/editor.cpp" "Source Files/util.cpp" -O1 -Wall -I include -L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

*These are no longer used to compile the game*

Instead now you have to use the CMake build system for compilations

# To do

    - CURRENT PRIORITY: SCREEN WRAPPING (DONE)

    - Enemy Collision, Slight tweaks to the enemy jump (WIP)

    - Fail State

    - Replacing the Game Editor with a new one
    - need to write the chicks, level complete and projectiles
    - Rewriting Game Engine, (Main Rewrite is complete)

----------------------------------------------------------------

========================================================================================
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

## Rewriting the engine
Starting out with **game.cpp** we have simplified the logic, we want the right functions in the right areas so I don't have to worry about everything and take too long in configurations and setups

After that we write the **renderer** that will handle all our **SDL** video functions so that we form an abstraction layer that never touches SDL

Then we made World which should load entities, levels, and assets for the current levels

```
main.cpp
 └── Game
        ├── Renderer
        ├── World
        │     ├── platforms
        │     ├── entities (Player, Enemy, Projectile)
        │     ├── loading + updating + rendering
        └── loop (processEvents → update → render)
```

So we implemented the Camera Model as it will be now and to be honest the engine is kind of looking quite a bit like our old one just instead slightly cleaner as less broken? Can't even be sure about that yet, good programming still has a long way to go. I need to be a lot more independent when it comes to programming.

```
GAME LOOP
   ├── update world
   └── render:
          renderer.beginFrame()
          renderer.renderWorld(world)
          renderer.endFrame()
```

After that we have been working on our animation player and changing the responsibilities of items. The game is basically being completely rewritten in the hope that this will make development a lot faster for this one and future game implementations

### Animation Player
Since our Sprite Sheets are irregular in their spacing and how they have the frames set

so we had 2 ways we could have done this that were sensible

Classic arcade games would manually set their frames and then have animations reference frame indices
```
sheet->frames = {
    {  0,  0, 16, 32 },
    { 18,  0, 16, 32 },
    { 36,  0, 15, 32 },
    { 52,  0, 16, 32 },
    // etc
};

walk.frames = {0,1,2,1};
idle.frames = {0};
```

or we could have the frame data saved in a JSON style format and then read the metadata
```
{
  "frames": {
    "walk_0": { "x":0,"y":0,"w":16,"h":32 },
    "walk_1": { "x":18,"y":0,"w":16,"h":32 }
  }
}
```

### 21st Dec 2025
So we have a bunch of systems that need to be extended and improved namely our rudimentary level loading system, need to see if our animatio system is complete. 
Finally we need to ensure that the player physics and interactions should be working.

So since we are recreating the player

**AABB Platformer Loop** : move->check->resolve->update

we have basically reconstructed movement and a lot faster than we did the first time around knowing we don't have to refactor a lot of code. I was much less experienced the last time, but this code is a lot better than what we had the first time around

### 4th January 2026
Been a while decide to work on the spawner. I think we might just keep the spawner as an entity that follows the same collision system and the like that the rest of our entities do

