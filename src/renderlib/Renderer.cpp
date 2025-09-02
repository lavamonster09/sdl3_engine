//
// Created by oliver on 21/08/2025.
//

#include "Renderer.h"
#include "../common.h"
#include "glm/glm.hpp"
#include <array>
#include <iostream>
#include <ostream>
#include <utility>

#include "../engine_types.h"
#include "Model.h"

void Renderer::init() {
    window = SDL_CreateWindow("name", 1600, 900, SDL_WINDOW_VULKAN);
    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
    SDL_ClaimWindowForGPUDevice(device, window);
    SDL_SetWindowRelativeMouseMode(window, true);
    SDL_SetWindowMouseGrab(window, true);

    vertex_shader = load_shader_from_file(device, "./shaders/vertex.vert.spv",
                                          VERTEX_SHADER, 1);
    fragment_shader = load_shader_from_file(device, "./shaders/fragment.frag.spv",
                                            FRAGMENT_SHADER, 0, 3);

    SDL_GPUGraphicsPipelineCreateInfo pipeline_info{
        .vertex_shader = vertex_shader,
        .fragment_shader = fragment_shader,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST
    };

    SDL_GPUVertexBufferDescription vertex_buffer_descriptions[1];
    vertex_buffer_descriptions[0].slot = 0;
    vertex_buffer_descriptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vertex_buffer_descriptions[0].instance_step_rate = 0;
    vertex_buffer_descriptions[0].pitch = sizeof(Vertex);

    pipeline_info.vertex_input_state.num_vertex_buffers = 1;
    pipeline_info.vertex_input_state.vertex_buffer_descriptions = vertex_buffer_descriptions;

    SDL_GPUVertexAttribute vertex_attributes[4];

    // a_position
    vertex_attributes[0].buffer_slot = 0;
    vertex_attributes[0].location = 0; // layout (location = 0)
    vertex_attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertex_attributes[0].offset = 0;

    // a_normal
    vertex_attributes[1].buffer_slot = 0;
    vertex_attributes[1].location = 1;
    vertex_attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertex_attributes[1].offset = sizeof(float) * 3;

    // a_uvs
    vertex_attributes[2].buffer_slot = 0;
    vertex_attributes[2].location = 2;
    vertex_attributes[2].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
    vertex_attributes[2].offset = sizeof(float) * 6;

    // a_tangents
    vertex_attributes[3].buffer_slot = 0;
    vertex_attributes[3].location = 3;
    vertex_attributes[3].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertex_attributes[3].offset = sizeof(float) * 8;

    pipeline_info.vertex_input_state.num_vertex_attributes = 4;
    pipeline_info.vertex_input_state.vertex_attributes = vertex_attributes;
    SDL_GPUColorTargetDescription color_target_descriptions[1];
    color_target_descriptions[0] = {};
    color_target_descriptions[0].format = SDL_GetGPUSwapchainTextureFormat(device, window);

    pipeline_info.target_info.num_color_targets = 1;
    pipeline_info.target_info.color_target_descriptions = color_target_descriptions;
    pipeline_info.target_info.has_depth_stencil_target = true;
    pipeline_info.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM;
    pipeline_info.depth_stencil_state = SDL_GPUDepthStencilState{
        .compare_op = SDL_GPU_COMPAREOP_LESS,
        .write_mask = 0xFF,
        .enable_depth_test = true,
        .enable_depth_write = true,
        .enable_stencil_test = false
    };

    pipeline_info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;

    SDL_GPUTextureCreateInfo texture_create_info{
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        .width = 1600,
        .height = 900,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1
    };

    depth_texture = SDL_CreateGPUTexture(
        device,
        &texture_create_info
    );

    graphics_pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipeline_info);

    SDL_GPUSamplerCreateInfo sampler_create_info{
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .mag_filter = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
    };

    sampler = SDL_CreateGPUSampler(device, &sampler_create_info);

    SDL_ReleaseGPUShader(device, vertex_shader);
    SDL_ReleaseGPUShader(device, fragment_shader);
}

void Renderer::draw(Camera *camera) {
    SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUTexture *swapchain_texture;
    Uint32 width, height;
    SDL_WaitAndAcquireGPUSwapchainTexture(
        command_buffer,
        window,
        &swapchain_texture,
        &width,
        &height
    );

    if (swapchain_texture == nullptr) {
        SDL_SubmitGPUCommandBuffer(command_buffer);
        return;
    }

    SDL_GPUColorTargetInfo color_target_info{
        .texture = swapchain_texture,
        .clear_color = {0.0f, 0.0f, 0.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };

    SDL_GPUDepthStencilTargetInfo depth_stencil_target_info{
        .texture = depth_texture,
        .clear_depth = 1,
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE,
        .stencil_load_op = SDL_GPU_LOADOP_CLEAR,
        .stencil_store_op = SDL_GPU_STOREOP_STORE,
        .cycle = true,
        .clear_stencil = 0,
    };

    SDL_GPURenderPass *render_pass = SDL_BeginGPURenderPass(
        command_buffer,
        &color_target_info,
        1,
        &depth_stencil_target_info
    );

    SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);

    for (auto &[key, model]: models) {
        model->draw(render_pass, sampler, camera, command_buffer, point_lights);
    }

    SDL_EndGPURenderPass(render_pass);

    SDL_SubmitGPUCommandBuffer(command_buffer);
}

void Renderer::add_model(ModelCreateInfo *info) {
    models[info->key] = new Model(device, info->model_path);
}

void Renderer::add_models(std::vector<ModelCreateInfo> &infos) {
    for (auto &info: infos) {
        add_model(&info);
    }
}

void Renderer::add_light(glm::vec3 position, glm::vec3 color, bool billboard, float strength) {
    point_lights.emplace_back(new PointLight{glm::vec4{position, 1.0f}, glm::vec4{color, strength}});
}
