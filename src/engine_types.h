//
// Created by oliver on 04/08/2025.
//

#ifndef VERTEX_H
#define VERTEX_H\

#include <glm/glm.hpp>

struct Vertex {
    float x, y, z;
    float r, g, b, a;
};

struct MeshBuffer {
    SDL_GPUBuffer *vertex_buffer;
    SDL_GPUTransferBufferLocation vertex_buffer_location;
    SDL_GPUBuffer *index_buffer;
    SDL_GPUTransferBufferLocation index_buffer_location;
    SDL_GPUTransferBuffer *transfer_buffer;
};

enum ShaderType {
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

struct ModelUniformBuffer {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    // you can add other properties here
};

#endif //VERTEX_H
