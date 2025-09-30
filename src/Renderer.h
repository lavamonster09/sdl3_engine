//
// Created by oliver on 30/09/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

#include <array>

#include "engine_types.h"

class Renderer {
public:
    void init();

    void create_window(const WindowCreateInfo &create_info);

    void draw();

private:
    Color clear_color_{};
    SDL_Window *window_ = nullptr;
    SDL_GPUDevice *device_ = nullptr;
    std::array<SDL_GPUShader *, 1> vertex_shaders_ = {};
    std::array<SDL_GPUShader *, 1> fragment_shaders_ = {};

    SDL_GPUShader *load_shader(const char *path, ShaderType shader_type, Uint8 num_uniform_buffers = 0,
                               Uint8 num_samplers = 0) const;
};


#endif //RENDERER_H
