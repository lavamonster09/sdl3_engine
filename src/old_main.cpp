#define SDL_MAIN_USE_CALLBACKS
#include <iostream>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "engine_types.h"
#include "InputManager.h"
#include "common.h"
#include <array>

#include "Camera.h"

SDL_Window *window;
SDL_GPUDevice *device;
InputManager input_manager;

double deltaTime = 0.0; // Time between current frame and last frame
Uint64 last_frame = 0.0; // Time of last frame
Uint64 current_frame = SDL_GetPerformanceCounter();

static std::array<Vertex, 36> vertices = {
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


static std::array<Uint32, 36> indexes = {
    0, 2, 1, 3, 5, 4, // back face
    6, 8, 7, 9, 11, 10, // front face
    12, 14, 13, 15, 17, 16, // left face
    18, 20, 19, 21, 23, 22, // right face
    24, 25, 26, 27, 28, 29, // bottom face
    30, 32, 31, 33, 35, 34 // top face
};


SDL_GPUBuffer *vertex_buffer;
SDL_GPUBuffer *index_buffer;
SDL_GPUTransferBuffer *transfer_buffer;
SDL_GPUCommandBuffer *command_buffer;
SDL_GPUCopyPass *copy_pass;

SDL_GPUShader *vertex_shader;
SDL_GPUShader *fragment_shader;

SDL_GPUGraphicsPipeline *graphics_pipeline;

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera;


SDL_GPUDevice *SDL_CreateGPUDevice(
    SDL_GPUShaderFormat format_flags,


    bool debug_mode,


    const char *name
);

struct UniformBuffer {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    // you can add other properties here
};

static UniformBuffer uniform_buffer{};

SDL_GPUShader *loadShaderFromFile(char *path, ShaderType shaderType, int num_uniform_buffers = 0) {
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

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    window = SDL_CreateWindow("SDL_3_ENGINE", 1600, 900, SDL_WINDOW_RESIZABLE);

    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
    SDL_ClaimWindowForGPUDevice(device, window);
    SDL_SetWindowRelativeMouseMode(window, true);
    SDL_SetWindowMouseGrab(window, true);

    // create the vertex buffer
    SDL_GPUBufferCreateInfo vertex_buffer_info{};
    vertex_buffer_info.size = sizeof(vertices);
    vertex_buffer_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertex_buffer = SDL_CreateGPUBuffer(device, &vertex_buffer_info);

    // create the index buffer
    SDL_GPUBufferCreateInfo index_buffer_info{
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = sizeof(indexes)
    };
    index_buffer = SDL_CreateGPUBuffer(device, &index_buffer_info);

    // create the transfer buffer
    SDL_GPUTransferBufferCreateInfo transfer_buffer_info{};
    transfer_buffer_info.size = sizeof(vertices) + sizeof(indexes);
    transfer_buffer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transfer_buffer_info);

    // put the vertex data in the transfer buffer
    Vertex *data = (Vertex *) SDL_MapGPUTransferBuffer(device, transfer_buffer, false);

    SDL_memcpy(data, vertices.data(), sizeof(vertices));
    Uint32 *index_data = (Uint32 *) &data[vertices.size()];
    SDL_memcpy(index_data, indexes.data(), sizeof(indexes));
    // index_data[0] = 0;
    // index_data[1] = 1;
    // index_data[2] = 2;
    // index_data[3] = 3;
    // index_data[4] = 4;
    // index_data[5] = 5;

    // setup the command buffer and begin a copy pass
    command_buffer = SDL_AcquireGPUCommandBuffer(device);
    copy_pass = SDL_BeginGPUCopyPass(command_buffer);
    // where is the data
    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;
    // where to place the data
    SDL_GPUBufferRegion region{};
    region.buffer = vertex_buffer;
    region.size = sizeof(vertices);
    region.offset = 0;

    // upload the vertex buffer
    SDL_UploadToGPUBuffer(copy_pass, &location, &region, false);


    SDL_GPUTransferBufferLocation index_buffer_location{};
    index_buffer_location.transfer_buffer = transfer_buffer;
    index_buffer_location.offset = sizeof(vertices);

    SDL_GPUBufferRegion index_buffer_region{};
    index_buffer_region.buffer = index_buffer;
    index_buffer_region.size = sizeof(indexes);
    index_buffer_region.offset = 0;

    // upload indexes
    SDL_UploadToGPUBuffer(copy_pass, &index_buffer_location, &index_buffer_region, false);

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(command_buffer);

    vertex_shader = loadShaderFromFile("C:/Users/oliver/CLionProjects/sdl3-engine/src/shaders/vertex.spv",
                                       VERTEX_SHADER, 1);
    fragment_shader = loadShaderFromFile("C:/Users/oliver/CLionProjects/sdl3-engine/src/shaders/fragment.spv",
                                         FRAGMENT_SHADER);

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
    pipeline_info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;

    graphics_pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipeline_info);

    SDL_ReleaseGPUShader(device, vertex_shader);
    SDL_ReleaseGPUShader(device, fragment_shader);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    last_frame = current_frame;
    current_frame = SDL_GetPerformanceCounter();
    deltaTime = (double) ((current_frame - last_frame) / (double) SDL_GetPerformanceFrequency());

    SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);

    SDL_GPUTexture *swapchain_texture;
    Uint32 width, height;
    SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, &width, &height);

    if (swapchain_texture == nullptr) {
        SDL_SubmitGPUCommandBuffer(command_buffer);
        return SDL_APP_CONTINUE;
    }

    SDL_GPUColorTargetInfo color_target_info{};
    color_target_info.clear_color = {1.0f, 1.0f, 1.0f, 1.0f};
    color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target_info.store_op = SDL_GPU_STOREOP_STORE;
    color_target_info.texture = swapchain_texture;

    SDL_GPURenderPass *render_pass = SDL_BeginGPURenderPass(command_buffer, &color_target_info, 1, nullptr);

    SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);

    SDL_GPUBufferBinding vertex_buffer_bindings[1];
    vertex_buffer_bindings[0].buffer = vertex_buffer;
    vertex_buffer_bindings[0].offset = 0;

    SDL_GPUBufferBinding index_buffer_bindings[1];
    index_buffer_bindings[0].buffer = index_buffer;
    index_buffer_bindings[0].offset = 0;

    SDL_BindGPUVertexBuffers(render_pass, 0, vertex_buffer_bindings, 1);
    SDL_BindGPUIndexBuffer(render_pass, index_buffer_bindings, SDL_GPU_INDEXELEMENTSIZE_16BIT);

    // create transformations
    // note that we're translating the scene in the reverse direction of where we want to move


    camera.update();

    uniform_buffer.view = camera.view;
    uniform_buffer.projection = camera.projection;
    uniform_buffer.model = glm::mat4(1.0f);
    SDL_PushGPUVertexUniformData(command_buffer, 0, &uniform_buffer, sizeof(UniformBuffer));
    // issue a draw call
    //SDL_DrawGPUPrimitives(render_pass, 6, 1, 0, 0);
    SDL_DrawGPUIndexedPrimitives(render_pass, indexes.size(), 1, 0, 0, 0);

    SDL_EndGPURenderPass(render_pass);

    SDL_SubmitGPUCommandBuffer(command_buffer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
        return SDL_APP_SUCCESS;
    }
    input_manager.handle_event(event);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyGPUDevice(device);
    SDL_ReleaseGPUBuffer(device, vertex_buffer);
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
    SDL_ReleaseGPUGraphicsPipeline(device, graphics_pipeline);
    SDL_DestroyWindow(window);
}
