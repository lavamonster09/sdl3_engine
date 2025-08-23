//
// Created by oliver on 05/08/2025.
//
#include "common.h"
#include <cmath>
#include <stdexcept>
#include <tiny_obj_loader.h>
#include <vector>
#include <iostream>

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
                              std::vector<Uint32> &indexes) {
    SDL_GPUBuffer *vertex_buffer = create_buffer(device, vertices.size() * sizeof(Vertex), SDL_GPU_BUFFERUSAGE_VERTEX);
    SDL_GPUBuffer *index_buffer = create_buffer(device, indexes.size() * sizeof(Uint32), SDL_GPU_BUFFERUSAGE_INDEX);
    SDL_GPUTransferBuffer *transfer_buffer = create_transfer_buffer(
        device, vertices.size() * sizeof(Vertex) + indexes.size() * sizeof(Uint32),
        SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD);
    Vertex *data = (Vertex *) SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    SDL_memcpy(data, vertices.data(), sizeof(Vertex) * vertices.size());
    Uint32 *index_data = (Uint32 *) &data[vertices.size()];
    SDL_memcpy(index_data, indexes.data(), sizeof(Uint32) * indexes.size());

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

SDL_GPUShader *load_shader_from_file(SDL_GPUDevice *device, std::string path, ShaderType shaderType,
                                     int num_uniform_buffers) {
    size_t shader_code_size;
    void *shader_code = SDL_LoadFile(path.c_str(), &shader_code_size);

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

std::tuple<MeshBuffer, std::vector<Vertex>, std::vector<Uint32> > create_mesh_buffer_from_path(
    SDL_GPUDevice *device, std::string path) {
    std::vector<Vertex> vertices;
    std::vector<Uint32> indexes;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;
    std::string warn;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, path.c_str())) {
        throw std::runtime_error(error);
    }

    for (const auto &shape: shapes) {
        for (const auto &index: shape.mesh.indices) {
            Vertex vertex{
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2],
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 0]
            };
            vertices.push_back(vertex);

            indexes.push_back(indexes.size());
        }
    }

    return {create_mesh_buffer(device, vertices, indexes), vertices, indexes};
}

