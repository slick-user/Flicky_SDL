cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.0.zip
)
FetchContent_MakeAvailable(catch2)

add_executable(FlickyTests test_basic.cpp)

# Correctly define CMAKE_SOURCE_DIR for tests
target_compile_definitions(FlickyTests PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(FlickyTests PRIVATE
    Catch2::Catch2WithMain
    FlickyEngine
    SDL3::SDL3
    SDL3_image::SDL3_image
)

include(CTest)
include(Catch)
catch_discover_tests(FlickyTests)
