//
// Created by oliver on 21/08/2025.
//

#include "Model.h"

#include "Camera.h"
#include "common.h"

Model::Model(SDL_GPUDevice *device, std::vector<Vertex> &vertices_, std::vector<Uint16> &indexes_, glm::mat4 model) {
    model_mat = model;
    vertices = vertices_;
    indexes = indexes_;
    mesh_buffer = create_mesh_buffer(device, vertices, indexes);


    SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(command_buffer);

    SDL_GPUBufferRegion vertex_buffer_region{};
    vertex_buffer_region.buffer = mesh_buffer.vertex_buffer;
    vertex_buffer_region.size = vertices.size() * sizeof(Vertex);
    vertex_buffer_region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &mesh_buffer.vertex_buffer_location, &vertex_buffer_region, false);

    SDL_GPUBufferRegion index_buffer_region{};
    index_buffer_region.buffer = mesh_buffer.index_buffer;
    index_buffer_region.size = indexes.size() * sizeof(Uint16);
    index_buffer_region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &mesh_buffer.index_buffer_location, &index_buffer_region, false);

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(command_buffer);
}

void Model::draw(SDL_GPURenderPass *render_pass, Camera &camera, SDL_GPUCommandBuffer *command_buffer) {
    SDL_GPUBufferBinding vertex_buffer_bindings[1];
    vertex_buffer_bindings[0].buffer = mesh_buffer.vertex_buffer;
    vertex_buffer_bindings[0].offset = 0;

    SDL_GPUBufferBinding index_buffer_bindings[1];
    index_buffer_bindings[0].buffer = mesh_buffer.index_buffer;
    index_buffer_bindings[0].offset = 0;

    SDL_BindGPUVertexBuffers(render_pass, 0, vertex_buffer_bindings, 1);
    SDL_BindGPUIndexBuffer(render_pass, index_buffer_bindings, SDL_GPU_INDEXELEMENTSIZE_16BIT);

    ModelUniformBuffer uniform_buffer{
        .model = model_mat,
        .view = camera.view,
        .projection = camera.projection
    };

    SDL_PushGPUVertexUniformData(command_buffer, 0, &uniform_buffer, sizeof(ModelUniformBuffer));

    SDL_DrawGPUIndexedPrimitives(render_pass, indexes.size(), 1, 0, 0, 0);
}
