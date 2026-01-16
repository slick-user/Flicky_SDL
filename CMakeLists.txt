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
    "${CMAKE_SOURCE_DIR}"
    ${SDL3_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
)

# Engine source files
file(GLOB_RECURSE ENGINE_SOURCES "source/*.cpp" "tools/*.cpp")

# -----------------------------
#  ImGui
# -----------------------------
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/lib/imgui")
set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
)

add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC 
    "${IMGUI_DIR}" 
    "${IMGUI_DIR}/backends"
)
target_link_libraries(ImGui PUBLIC SDL3::SDL3)

# -----------------------------
#  Build Engine as a library
# -----------------------------
add_library(FlickyEngine ${ENGINE_SOURCES})

target_link_libraries(FlickyEngine PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)
target_link_libraries(FlickyEngine PUBLIC ImGui)

target_include_directories(FlickyEngine PUBLIC include "${CMAKE_SOURCE_DIR}")

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
