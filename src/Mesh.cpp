//
// Created by oliver on 27/08/2025.
//

#include "Mesh.h"

#include <utility>

#include "common.h"

void Mesh::setup_mesh(SDL_GPUDevice *device) {
    mesh_buffer = create_mesh_buffer(device, vertices, indices);
}

void Mesh::upload_mesh(SDL_GPUDevice *device) {
    SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(command_buffer);

    SDL_GPUBufferRegion vertex_buffer_region{};
    vertex_buffer_region.buffer = mesh_buffer.vertex_buffer;
    vertex_buffer_region.size = vertices.size() * sizeof(Vertex);
    vertex_buffer_region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &mesh_buffer.vertex_buffer_location, &vertex_buffer_region, false);

    SDL_GPUBufferRegion index_buffer_region{};
    index_buffer_region.buffer = mesh_buffer.index_buffer;
    index_buffer_region.size = indices.size() * sizeof(Uint32);
    index_buffer_region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &mesh_buffer.index_buffer_location, &index_buffer_region, false);
    SDL_GPUTextureRegion texture_buffer_region{};
    texture_buffer_region.texture = texture.texture;
    texture_buffer_region.w = texture.image_data->w;
    texture_buffer_region.h = texture.image_data->h;
    texture_buffer_region.d = 1;

    SDL_UploadToGPUTexture(copy_pass, &texture.texture_transfer_info, &texture_buffer_region, false);

    SDL_GPUTextureRegion n_texture_buffer_region{};
    n_texture_buffer_region.texture = ntexture.texture;
    n_texture_buffer_region.w = ntexture.image_data->w;
    n_texture_buffer_region.h = ntexture.image_data->h;
    n_texture_buffer_region.d = 1;

    SDL_UploadToGPUTexture(copy_pass, &ntexture.texture_transfer_info, &n_texture_buffer_region, false);

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(command_buffer);
    SDL_DestroySurface(texture.image_data);
    SDL_ReleaseGPUTransferBuffer(device, mesh_buffer.transfer_buffer);
    SDL_ReleaseGPUTransferBuffer(device, texture.texture_transfer_buffer);
}

Mesh::Mesh(SDL_GPUDevice *device, std::vector<Vertex> vertices, std::vector<Uint32> indices, Texture texture, Texture ntexture) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->texture = texture;
    this->ntexture = ntexture;

    setup_mesh(device);
    upload_mesh(device);
}

void Mesh::draw(SDL_GPURenderPass *render_pass, SDL_GPUSampler *sampler, const Camera &camera, SDL_GPUCommandBuffer *command_buffer, const std::vector<PointLight *> &lights, glm::mat4 model_mat) {
    SDL_GPUBufferBinding vertex_buffer_bindings[1];
    vertex_buffer_bindings[0].buffer = mesh_buffer.vertex_buffer;
    vertex_buffer_bindings[0].offset = 0;

    SDL_GPUBufferBinding index_buffer_bindings[1];
    index_buffer_bindings[0].buffer = mesh_buffer.index_buffer;
    index_buffer_bindings[0].offset = 0;

    SDL_GPUTextureSamplerBinding texture_sampler_bindings[2];
    texture_sampler_bindings[0].texture = texture.texture;
    texture_sampler_bindings[0].sampler = sampler;
    texture_sampler_bindings[1].texture = ntexture.texture;
    texture_sampler_bindings[1].sampler = sampler;

    SDL_BindGPUVertexBuffers(render_pass, 0, vertex_buffer_bindings, 1);
    SDL_BindGPUIndexBuffer(render_pass, index_buffer_bindings, SDL_GPU_INDEXELEMENTSIZE_32BIT);

    ModelUniformBuffer uniform_buffer{
        .model = model_mat,
        .view = camera.view,
        .projection = camera.projection,
        .view_pos = glm::vec4{camera.pos, 0.0f},
    };

    for (int i = 0; i < lights.size(); i++) {
        uniform_buffer.lights[i].color = lights[i]->color;
        uniform_buffer.lights[i].pos = lights[i]->pos;
    }

    SDL_BindGPUFragmentSamplers(render_pass, 0, texture_sampler_bindings, 2);
    SDL_PushGPUVertexUniformData(command_buffer, 0, &uniform_buffer, sizeof(ModelUniformBuffer));
    SDL_DrawGPUIndexedPrimitives(render_pass, indices.size(), 1, 0, 0, 0);
}
