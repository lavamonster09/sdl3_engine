//
// Created by oliver on 05/08/2025.
//
#include "common.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <SDL3_image/SDL_image.h>

#include "renderlib/Model.h"

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
                                     int num_uniform_buffers, int num_samplers) {
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
    shader_info.num_samplers = num_samplers;
    shader_info.num_storage_buffers = 0;
    shader_info.num_storage_textures = 0;
    shader_info.num_uniform_buffers = num_uniform_buffers;
    SDL_GPUShader *shader = SDL_CreateGPUShader(device, &shader_info);
    SDL_free(shader_code);
    return shader;
}

Texture *create_texture_image(
    SDL_GPUDevice *device, const std::string &path) {
    std::string p = "../assets/" + path;
    SDL_Surface *image_data = LoadImage(p.c_str(), 4);

    SDL_GPUTextureCreateInfo texture_create_info{};
    texture_create_info.type = SDL_GPU_TEXTURETYPE_2D;
    texture_create_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texture_create_info.width = image_data->w;
    texture_create_info.height = image_data->h;
    texture_create_info.layer_count_or_depth = 1;
    texture_create_info.num_levels = 1;
    texture_create_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;

    SDL_GPUTransferBuffer *transfer_buffer = create_transfer_buffer(
        device, image_data->w * image_data->h * 4,
        SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD);

    Uint8 *transfer_ptr = (Uint8 *) SDL_MapGPUTransferBuffer(
        device,
        transfer_buffer,
        false
    );

    SDL_GPUTextureTransferInfo texture_transfer_info{};
    texture_transfer_info.transfer_buffer = transfer_buffer;
    texture_transfer_info.offset = 0;

    SDL_memcpy(transfer_ptr, image_data->pixels, image_data->w * image_data->h * 4);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUTexture *texture = SDL_CreateGPUTexture(device, &texture_create_info);

    return new Texture{image_data, texture, texture_transfer_info};
}


SDL_Surface *LoadImage(const char *path, int desiredChannels) {
    SDL_Surface *result;
    SDL_PixelFormat format;

    result = IMG_Load(path);
    if (result == NULL) {
        SDL_Log("Failed to load IMAGE: %s path:%s", SDL_GetError(), path);
        return NULL;
    }

    if (desiredChannels == 4) {
        format = SDL_PIXELFORMAT_ABGR8888;
    } else {
        SDL_assert(!"Unexpected desiredChannels");
        SDL_DestroySurface(result);
        return NULL;
    }
    if (result->format != format) {
        SDL_Surface *next = SDL_ConvertSurface(result, format);
        SDL_DestroySurface(result);
        result = next;
    }

    return result;
}
