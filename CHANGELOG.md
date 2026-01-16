# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.1] - 

### Added
- Renderer System for drawing abstraction
- Testing Framework (Catch2)
- CMake Build System
- Standalone Camera system
- Animation Player for spritesheet handling
- Enemy AI implementation
- Spawner entities
- Entity collision system
- Projectile system
- Added Chirps
- ImGui Integration
- Level Editor

### Changed
- Engine layout and game.cpp flow
- Reduced game.cpp responsibilities by adding world.cpp
- Physics and collision system refactor
- Removed Utils dependency
- Screen wrapping implementation
- Swapped out Raw Pointers entities array for Smart Pointers
- Changed Level Loading
- Level Loading

### Fixed
- 

## [0.2.0]
- C++ rewrite from C prototype

## [0.1.0]
- Initial C prototype

# Reasoning behind Version Control
This is considering `0.1` to have been the prototype that was originally written in C. 
`0.2` to have been the rewrite in C++
`0.3` is after refactoring the game for SDL3 (in place of SDL2) then the Rewrite of the complete engine
