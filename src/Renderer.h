//
// Created by oliver on 21/08/2025.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

#include "Camera.h"
#include "engine_types.h"


class Renderer {
    SDL_Window *window = nullptr;
    SDL_GPUDevice *device = nullptr;
    SDL_GPUShader *vertex_shader = nullptr;
    SDL_GPUShader *fragment_shader = nullptr;
    SDL_GPUGraphicsPipeline *graphics_pipeline = nullptr;
    ModelUniformBuffer uniform_buffer = {};
    SDL_GPUTexture *depth_texture = nullptr;

public:
    void init();

    void draw(Camera &camera);

    // create buffers
};


#endif //RENDERER_H
