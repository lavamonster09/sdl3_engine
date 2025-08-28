project(renderlib)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(assimp REQUIRED)

set(RENDER_LIB_SRC
        Camera.cpp
        Camera.h
        Mesh.cpp
        Mesh.h
        Model.h
        Model.cpp
        Renderer.cpp
        Renderer.h
)

add_library(${PROJECT_NAME} ${RENDER_LIB_SRC})

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} assimp::assimp)
target_link_libraries(${PROJECT_NAME} $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)
