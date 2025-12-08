add_compile_definitions(PROJECT_ROOT=\"${CMAKE_SOURCE_DIR}\")

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

# Engine source files
file(GLOB_RECURSE ENGINE_SOURCES "source/*.cpp")

# -----------------------------
#  Build Engine as a library
# -----------------------------
add_library(FlickyEngine ${ENGINE_SOURCES})

target_link_libraries(FlickyEngine PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

target_include_directories(FlickyEngine PUBLIC include)

# -----------------------------
#  Build main executable
# -----------------------------
add_executable(Flicky main.cpp)
target_link_libraries(Flicky PRIVATE FlickyEngine)

# -----------------------------
# Enable tests
# -----------------------------
enable_testing()
add_subdirectory(test)
