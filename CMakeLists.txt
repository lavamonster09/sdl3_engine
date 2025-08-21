cmake_minimum_required(VERSION 3.31)
project(sdl3_engine)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


add_executable(sdl3_engine src/main.cpp
        src/vertex.h
        src/common.h
        src/common.cpp
        src/camera.cpp
        src/camera.h
        src/InputManager.cpp
        src/InputManager.h
)

target_link_libraries(sdl3_engine PRIVATE SDL3::SDL3)
target_link_libraries(sdl3_engine PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)
target_link_libraries(sdl3_engine PRIVATE SDL3_ttf::SDL3_ttf)
target_link_libraries(sdl3_engine PRIVATE glm::glm)