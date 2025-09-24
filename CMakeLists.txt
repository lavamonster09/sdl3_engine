cmake_minimum_required(VERSION 3.31)
project(sdl3_engine)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)

include_directories(src/renderlib)

add_executable(sdl3_engine src/main.cpp
        src/engine_types.h
        src/common.h
        src/common.cpp
        src/InputManager.cpp
        src/InputManager.h
        src/Engine.cpp
        src/Engine.h
        src/CameraController.cpp
        src/CameraController.h
        src/ECS.h
        src/components.h
        src/Renderer3DSystem.cpp
        src/Renderer3DSystem.h
        src/renderlib/AssetManager.cpp
        src/renderlib/AssetManager.h
        src/renderlib/Camera.cpp
        src/renderlib/Camera.h
        src/renderlib/Mesh.cpp
        src/renderlib/Mesh.h
        src/renderlib/Model.h
        src/renderlib/Model.cpp
        src/renderlib/Renderer.cpp
        src/renderlib/Renderer.h
)

function(add_shaders TARGET_NAME)
    set(SHADER_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
    set(SHADER_SOURCES ${ARGN})
    set(SHADER_PRODUCTS)
    add_custom_command(
            OUTPUT ${SHADER_OUT_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUT_DIR}
    )

    foreach (SHADER_SOURCE IN LISTS SHADER_SOURCES)
        get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME)
        set(SHADER_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER_NAME}.spv")

        add_custom_command(OUTPUT ${SHADER_OUTPUT_PATH}
                COMMAND glslc ${SHADER_SOURCE} -o ${SHADER_OUTPUT_PATH}
                DEPENDS ${SHADER_SOURCE}
                COMMENT "Compiling ${SHADER_NAME}"
        )
        list(APPEND SHADER_PRODUCTS ${SHADER_OUTPUT_PATH})
    endforeach ()

    add_custom_target(${TARGET_NAME} ALL DEPENDS ${SHADER_PRODUCTS})
endfunction()

file(GLOB SHADER_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/shaders/*")
add_shaders(Shaders ${SHADER_SOURCES})
add_dependencies(sdl3_engine Shaders)

target_link_libraries(sdl3_engine PRIVATE SDL3::SDL3)
target_link_libraries(sdl3_engine PRIVATE tinyobjloader::tinyobjloader)
target_link_libraries(sdl3_engine PRIVATE glm::glm)
target_link_libraries(sdl3_engine PRIVATE assimp::assimp)
target_link_libraries(sdl3_engine PRIVATE imgui::imgui)
target_link_libraries(sdl3_engine PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)
target_include_directories(sdl3_engine PRIVATE ${Stb_INCLUDE_DIR})