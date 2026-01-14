# Compilation

g++ -o Flicky main.cpp "Source Files/game.cpp" "Source Files/player.cpp" "Source Files/util.cpp" "Source Files/entity.cpp" "Source Files/enemy.cpp" "Source Files/projectile.cpp" -O1 -Wall -I include -L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

For the Editor version:
g++ -o Flicky main.cpp "Source Files/editor.cpp" "Source Files/util.cpp" -O1 -Wall -I include -L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

*These are no longer used to compile the game*

Instead now you have to use the CMake build system for compilations

# To do

    - Write Projectile (DONE)
    - Write Chicks (DONE)
    - Fail State (Will possibly be completed at a later date)

    - Replacing the Game Editor with a new one
    - level complete (DONE) 
    - Rewriting Game Engine, (Main Rewrite is complete)

----------------------------------------------------------------

========================================================================================


# Stuff I am learning about during the process

## Version Control
I think I just found out about one of the best additions I can have to my process. Implementing and using version control. One way would be Having multiple branches 

### Branch structure
Still have not implemented this and we might not... It was a nice thought though

'main'          # Stable, production-ready code
'develop'       # Integration branch for features
'feature/*'     # Individual features (feature/collision-system)
'hotfix/*'      # Emergency fixes (hotfix/crash-on-startup)
'release/*'     # Release preparation (release/v0.4.0)

### Possible Workflow
#### Start a new feature
git checkout develop
git pull origin develop
git checkout -b feature/enemy-ai-improvements

#### Work on feature, commit frequently
git add .
git commit -m "feat: improve enemy ledge detection logic"

#### When done, merge to develop
git checkout develop
git merge --no-ff feature/enemy-ai-improvements
git push origin develop

#### When ready for release
git checkout main
git merge --no-ff develop
git tag -a v0.4.0 -m "Release v0.4.0"
git push origin main --tags

#### Commit Conventions
# Format: <type>(<scope>): <subject>

#### Types:
feat:     # New feature
fix:      # Bug fix
docs:     # Documentation
style:    # Formatting, no code change
refactor: # Code restructuring
test:     # Adding tests
chore:    # Build, dependencies, etc.

#### Examples:
feat(collision): add player-enemy collision detection
fix(ai): enemy no longer falls through platforms
refactor(renderer): extract sprite rendering to separate class
docs(readme): add build instructions
test(entity): add unit tests for collision detection

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

Camera does exactly what it is supposed completely functional

```
GAME LOOP
   ├── update world
   └── render:
          renderer.beginFrame()
          renderer.renderWorld(world)
          renderer.endFrame()
```

After that we have been working on our animation player and changing the responsibilities of items. The game is basically being completely rewritten in the hope that this will make development a lot faster for this one and future game implementations. That was not the case, our rewrite took longer than if we were primarily focused on simply making the game

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

we decided that maving the frame data and editing it that way was the way to go

### Level Loading 
Level loading still uses .txt files we just made the data in our levels more verbose, the level loading also now works for all sorts of entities

Need to integrate the level saving with the level editing tool that we are going to implement and that should complete our dev flow

### Physics
So since we are recreating the player

**AABB Platformer Loop** : move->check->resolve->update

we have basically reconstructed movement and a lot faster than we did the first time around knowing we don't have to refactor a lot of code. I was much less experienced the last time, but this code is a lot better than what we had the 

Been a while decide to work on the spawner. I think we might just keep the spawner as an entity that follows the same collision system and the like that the rest of our entities do.

The engine itself will be handling all of the collisions instead of the entities dealing with the collisions

