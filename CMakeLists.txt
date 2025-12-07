set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.20)
project(Flicky CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where your SDL3 config is located
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib/cmake")

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

include_directories(
    include
    ${SDL3_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
)

file(GLOB_RECURSE SOURCES
    "source/*.cpp"
)

add_executable(Flicky main.cpp ${SOURCES})

target_link_libraries(Flicky PRIVATE 
  SDL3::SDL3
  SDL3_image::SDL3_image
)
