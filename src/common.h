//
// Created by oliver on 05/08/2025.
//

#ifndef COMMON_H
#define COMMON_H
#include <array>
#include <vector>
#include <SDL3/SDL.h>

#include "engine_types.h"

SDL_GPUBuffer *create_buffer(SDL_GPUDevice *device, Uint32 size, SDL_GPUBufferUsageFlags usage);

SDL_GPUTransferBuffer *create_transfer_buffer(SDL_GPUDevice *device, Uint32 size, SDL_GPUTransferBufferUsage usage);

MeshBuffer create_mesh_buffer(SDL_GPUDevice *device, std::vector<Vertex> &vertices,
                              std::vector<Uint32> &indexes);

SDL_GPUShader *load_shader_from_file(SDL_GPUDevice *device, char *path, ShaderType shaderType,
                                     int num_uniform_buffers = 0);

std::tuple<MeshBuffer, std::vector<Vertex>, std::vector<Uint32> > create_mesh_buffer_from_path(
    SDL_GPUDevice *device, char *path);
#endif //COMMON_H
