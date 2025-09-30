// // Created by oliver on 30/09/2025.
//

#include "Renderer.h"

#include <iostream>

void Renderer::init() {
    if (device_ == nullptr) {
        std::cout << "[ENGINE::ERROR] config has not been initialised" << std::endl;
    }
    SDL_ClaimWindowForGPUDevice(device_, window_);
    vertex_shaders_[0] = load_shader("./shaders/vertex.vert.spv", VERTEX_SHADER, 1);
    fragment_shaders_[0] = load_shader("./shaders/fragment.frag.spv", FRAGMENT_SHADER, 0, 3);
}

void Renderer::create_window(const WindowCreateInfo &create_info) {
    clear_color_ = create_info.clear_color;
    window_ = SDL_CreateWindow(
        create_info.window_name,
        create_info.w,
        create_info.h,
        SDL_WINDOW_VULKAN
    );

    device_ = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV,
        true,
        nullptr
    );
}

void Renderer::draw() {
    return;
}

SDL_GPUShader *Renderer::load_shader(const char *path, const ShaderType shader_type,
                                     const Uint8 num_uniform_buffers, const Uint8 num_samplers) const {
    size_t shader_code_size;
    void *shader_code = SDL_LoadFile(path, &shader_code_size);
    SDL_GPUShaderStage shader_stage = {};
    if (shader_type == VERTEX_SHADER) {
        shader_stage = SDL_GPU_SHADERSTAGE_VERTEX;
    } else {
        shader_stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }
    SDL_GPUShaderCreateInfo shader_create_info = {
        shader_code_size,
        (Uint8 *) shader_code,
        "main",
        SDL_GPU_SHADERFORMAT_SPIRV,
        shader_stage,
        num_samplers,
        0,
        0,
        num_uniform_buffers
    };
    SDL_GPUShader *shader = SDL_CreateGPUShader(device_, &shader_create_info);
    SDL_free(shader_code);
    return shader;
}
