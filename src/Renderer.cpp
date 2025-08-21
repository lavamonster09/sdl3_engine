//
// Created by oliver on 21/08/2025.
//

#include "Renderer.h"
#include "common.h"
#include "glm/glm.hpp"
#include <array>
#include <glm/ext/quaternion_transform.hpp>

#include "engine_types.h"
#include "Model.h"

static std::vector<Vertex> vertices = {
    // Back face (Z-)
    Vertex{-0.5f, -0.5f, -0.5f, 1, 0, 0, 1},
    {0.5f, -0.5f, -0.5f, 0, 1, 0, 1},
    {0.5f, 0.5f, -0.5f, 0, 0, 1, 1},

    {0.5f, 0.5f, -0.5f, 0, 0, 1, 1},
    {-0.5f, 0.5f, -0.5f, 1, 1, 0, 1},
    {-0.5f, -0.5f, -0.5f, 1, 0, 0, 1},

    // Front face (Z+)
    {-0.5f, -0.5f, 0.5f, 1, 0, 0, 1},
    {0.5f, 0.5f, 0.5f, 0, 1, 0, 1},
    {0.5f, -0.5f, 0.5f, 0, 0, 1, 1},
    {0.5f, 0.5f, 0.5f, 0, 1, 0, 1},
    {-0.5f, -0.5f, 0.5f, 1, 0, 0, 1},
    {-0.5f, 0.5f, 0.5f, 1, 1, 0, 1},
    // Left face (X-)
    {-0.5f, -0.5f, -0.5f, 1, 0, 0, 1},
    {-0.5f, 0.5f, -0.5f, 0, 1, 0, 1},
    {-0.5f, 0.5f, 0.5f, 0, 0, 1, 1},
    {-0.5f, 0.5f, 0.5f, 0, 0, 1, 1},
    {-0.5f, -0.5f, 0.5f, 1, 1, 0, 1},
    {-0.5f, -0.5f, -0.5f, 1, 0, 0, 1},
    // Right face (X+)
    {0.5f, -0.5f, -0.5f, 1, 0, 0, 1},
    {0.5f, 0.5f, 0.5f, 0, 1, 0, 1},
    {0.5f, 0.5f, -0.5f, 0, 0, 1, 1},
    {0.5f, 0.5f, 0.5f, 0, 1, 0, 1},
    {0.5f, -0.5f, -0.5f, 1, 0, 0, 1},
    {0.5f, -0.5f, 0.5f, 1, 1, 0, 1},
    // Bottom face (Y-)
    {-0.5f, -0.5f, -0.5f, 1, 0, 0, 1},
    {0.5f, -0.5f, -0.5f, 0, 1, 0, 1},
    {0.5f, -0.5f, 0.5f, 0, 0, 1, 1},
    {0.5f, -0.5f, 0.5f, 0, 0, 1, 1},
    {-0.5f, -0.5f, 0.5f, 1, 1, 0, 1},
    {-0.5f, -0.5f, -0.5f, 1, 0, 0, 1},

    // Top face (Y+)
    {-0.5f, 0.5f, -0.5f, 1, 0, 0, 1},
    {0.5f, 0.5f, -0.5f, 0, 1, 0, 1},
    {0.5f, 0.5f, 0.5f, 0, 0, 1, 1},

    {0.5f, 0.5f, 0.5f, 0, 0, 1, 1},
    {-0.5f, 0.5f, 0.5f, 1, 1, 0, 1},
    {-0.5f, 0.5f, -0.5f, 1, 0, 0, 1}
};


static std::vector<Uint16> indexes = {
    0, 2, 1, 3, 5, 4, // back face
    6, 8, 7, 9, 11, 10, // front face
    12, 14, 13, 15, 17, 16, // left face
    18, 20, 19, 21, 23, 22, // right face
    24, 25, 26, 27, 28, 29, // bottom face
    30, 32, 31, 33, 35, 34 // top face
};

struct UniformBuffer {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    // you can add other properties here
};

void Renderer::init() {
    window = SDL_CreateWindow("name", 1600, 900, SDL_WINDOW_RESIZABLE);
    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
    SDL_ClaimWindowForGPUDevice(device, window);
    SDL_SetWindowRelativeMouseMode(window, true);
    SDL_SetWindowMouseGrab(window, true);

    vertex_shader = load_shader_from_file(device, "C:/Users/oliver/CLionProjects/sdl3-engine/src/shaders/vertex.spv",
                                          VERTEX_SHADER, 1);
    fragment_shader = load_shader_from_file(
        device, "C:/Users/oliver/CLionProjects/sdl3-engine/src/shaders/fragment.spv",
        FRAGMENT_SHADER, 0);

    SDL_GPUGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.vertex_shader = vertex_shader;
    pipeline_info.fragment_shader = fragment_shader;

    pipeline_info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    SDL_GPUVertexBufferDescription vertex_buffer_descriptions[1];
    vertex_buffer_descriptions[0].slot = 0;
    vertex_buffer_descriptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vertex_buffer_descriptions[0].instance_step_rate = 0;
    vertex_buffer_descriptions[0].pitch = sizeof(Vertex);

    pipeline_info.vertex_input_state.num_vertex_buffers = 1;
    pipeline_info.vertex_input_state.vertex_buffer_descriptions = vertex_buffer_descriptions;

    SDL_GPUVertexAttribute vertex_attributes[2];

    // a_position
    vertex_attributes[0].buffer_slot = 0;
    vertex_attributes[0].location = 0; // layout (location = 0)
    vertex_attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertex_attributes[0].offset = 0;

    // a_color
    vertex_attributes[1].buffer_slot = 0;
    vertex_attributes[1].location = 1;
    vertex_attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
    vertex_attributes[1].offset = sizeof(float) * 3;

    pipeline_info.vertex_input_state.num_vertex_attributes = 2;
    pipeline_info.vertex_input_state.vertex_attributes = vertex_attributes;

    SDL_GPUColorTargetDescription color_target_descriptions[1];
    color_target_descriptions[0] = {};
    color_target_descriptions[0].format = SDL_GetGPUSwapchainTextureFormat(device, window);

    pipeline_info.target_info.num_color_targets = 1;
    pipeline_info.target_info.color_target_descriptions = color_target_descriptions;
    pipeline_info.target_info.has_depth_stencil_target = true;
    pipeline_info.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM;
    pipeline_info.depth_stencil_state = (SDL_GPUDepthStencilState){
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

    SDL_ReleaseGPUShader(device, vertex_shader);
    SDL_ReleaseGPUShader(device, fragment_shader);
}

void Renderer::draw(Camera &camera) {
    Model cube{device, vertices, indexes, glm::mat4(1.0f)};
    Model cube2{device, vertices, indexes, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 0.0f))};

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
        .clear_color = {0.1f, 0.1f, 0.1f},
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

    camera.update();

    cube.draw(render_pass, camera, command_buffer);
    cube2.draw(render_pass, camera, command_buffer);

    SDL_EndGPURenderPass(render_pass);

    SDL_SubmitGPUCommandBuffer(command_buffer);
}
