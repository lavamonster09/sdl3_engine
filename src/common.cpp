//
// Created by oliver on 05/08/2025.
//
#include "common.h"
#include <cmath>
#include <vector>

SDL_GPUBuffer *create_buffer(SDL_GPUDevice *device, Uint32 size, SDL_GPUBufferUsageFlags usage) {
    SDL_GPUBufferCreateInfo buffer_info{
        .usage = usage,
        .size = size
    };
    return SDL_CreateGPUBuffer(device, &buffer_info);
}

SDL_GPUTransferBuffer *create_transfer_buffer(SDL_GPUDevice *device, Uint32 size, SDL_GPUTransferBufferUsage usage) {
    SDL_GPUTransferBufferCreateInfo buffer_info{
        .usage = usage,
        .size = size
    };
    return SDL_CreateGPUTransferBuffer(device, &buffer_info);
}

MeshBuffer create_mesh_buffer(SDL_GPUDevice *device, std::vector<Vertex> &vertices,
                              std::vector<Uint16> &indexes) {
    SDL_GPUBuffer *vertex_buffer = create_buffer(device, vertices.size() * sizeof(Vertex), SDL_GPU_BUFFERUSAGE_VERTEX);
    SDL_GPUBuffer *index_buffer = create_buffer(device, indexes.size() * sizeof(Uint16), SDL_GPU_BUFFERUSAGE_INDEX);
    SDL_GPUTransferBuffer *transfer_buffer = create_transfer_buffer(
        device, vertices.size() * sizeof(Vertex) + indexes.size() * sizeof(Uint16),
        SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD);
    Vertex *data = (Vertex *) SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    SDL_memcpy(data, vertices.data(), sizeof(Vertex) * vertices.size());
    Uint16 *index_data = (Uint16 *) &data[vertices.size()];
    SDL_memcpy(index_data, indexes.data(), sizeof(Uint16) * indexes.size());

    SDL_GPUTransferBufferLocation vertex_buffer_location{};
    vertex_buffer_location.transfer_buffer = transfer_buffer;
    vertex_buffer_location.offset = 0;

    SDL_GPUTransferBufferLocation index_buffer_location{};
    index_buffer_location.transfer_buffer = transfer_buffer;
    index_buffer_location.offset = vertices.size() * sizeof(Vertex);

    return MeshBuffer{
        .vertex_buffer = vertex_buffer,
        .vertex_buffer_location = vertex_buffer_location,
        .index_buffer = index_buffer,
        .index_buffer_location = index_buffer_location,
        .transfer_buffer = transfer_buffer
    };
}

SDL_GPUShader *load_shader_from_file(SDL_GPUDevice *device, char *path, ShaderType shaderType,
                                     int num_uniform_buffers) {
    size_t shader_code_size;
    void *shader_code = SDL_LoadFile(path, &shader_code_size);

    SDL_GPUShaderCreateInfo shader_info{};
    shader_info.code = (Uint8 *) shader_code;
    shader_info.code_size = shader_code_size;
    shader_info.entrypoint = "main";
    shader_info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    if (shaderType == VERTEX_SHADER) {
        shader_info.stage = SDL_GPU_SHADERSTAGE_VERTEX;
    } else if (shaderType == FRAGMENT_SHADER) {
        shader_info.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }
    shader_info.num_samplers = 0;
    shader_info.num_storage_buffers = 0;
    shader_info.num_storage_textures = 0;
    shader_info.num_uniform_buffers = num_uniform_buffers;
    SDL_GPUShader *shader = SDL_CreateGPUShader(device, &shader_info);
    SDL_free(shader_code);
    return shader;
}
