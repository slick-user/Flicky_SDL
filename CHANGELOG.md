#ChangeLog

## [0.3.1] - 2026-01-03
### Added
    - Added the Renderer System to deal with drawing from now on
    - Added a Testing Frame Work
    - Implemented a Build System to make game compilation easier and smoother
    - Added a standalone Camera
    - Added an Animation Player to handle spritesheets handling
    - Completed Enemy AI
    - Added Spawners
### Changed
    - Changed the engine layout, how game.cpp handles flow, order of events, Entitie loading
    - Reduced the responsibilities of **game.cpp**, by adding **world.cpp**
    - Changed the physics and Collision system
    - Removed the usage of **Utils**
    - Added Enemy Assets
    - Changed Screen Wrapping
### Fixed

# Reasoning behind Version Control
This is considering `0.1` to have been the prototype that was originally written in C. 
`0.2` to have been the rewrite in C++
`0.3` is after refactoring the game for SDL3 (in place of SDL2) then the Rewrite of the complete engine
