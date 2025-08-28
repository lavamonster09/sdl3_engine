//
// Created by oliver on 04/08/2025.
//

#ifndef VERTEX_H
#define VERTEX_H\

#include <glm/glm.hpp>
#include <string>

struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float uvx, uvy;
    float tx, ty, tz;
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

struct PointLight {
    glm::vec4 pos;
    glm::vec4 color;
};

struct LightInfo {
    glm::vec3 position;
    glm::vec3 color;
    bool billboard;
    float strength;
};

struct ModelUniformBuffer {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 view_pos;
    PointLight lights[4];
    // you can add other properties here
};

struct Texture {
    SDL_Surface *image_data;
    SDL_GPUTexture *texture;
    SDL_GPUTransferBuffer *texture_transfer_buffer;
    SDL_GPUTextureTransferInfo texture_transfer_info;
};

struct ModelCreateInfo {
    std::string key;
    std::string model_path;
    std::string texture_path;
    std::string normal_path;
    std::string rough_path;
};


#endif //VERTEX_H
