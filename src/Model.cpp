//
// Created by oliver on 21/08/2025.
//

#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <tiny_obj_loader.h>

#include "Camera.h"
#include "common.h"

void Model::upload_buffers_(SDL_GPUDevice *device) {
    SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(command_buffer);

    SDL_GPUBufferRegion vertex_buffer_region{};
    vertex_buffer_region.buffer = mesh_buffer.vertex_buffer;
    vertex_buffer_region.size = vertices.size() * sizeof(Vertex);
    vertex_buffer_region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &mesh_buffer.vertex_buffer_location, &vertex_buffer_region, false);

    SDL_GPUBufferRegion index_buffer_region{};
    index_buffer_region.buffer = mesh_buffer.index_buffer;
    index_buffer_region.size = indexes.size() * sizeof(Uint32);
    index_buffer_region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &mesh_buffer.index_buffer_location, &index_buffer_region, false);
    SDL_GPUTextureRegion texture_buffer_region{};
    texture_buffer_region.texture = texture;
    texture_buffer_region.w = image_data->w;
    texture_buffer_region.h = image_data->h;
    texture_buffer_region.d = 1;

    SDL_UploadToGPUTexture(copy_pass, &texture_transfer_info, &texture_buffer_region, false);

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(command_buffer);
    SDL_DestroySurface(image_data);
    SDL_ReleaseGPUTransferBuffer(device, mesh_buffer.transfer_buffer);
    SDL_ReleaseGPUTransferBuffer(device, texture_transfer_buffer);
}

Model::Model(SDL_GPUDevice *device, std::vector<Vertex> &vertices_, std::vector<Uint32> &indexes_, glm::vec3 pos,
             float rotation, glm::vec3 axis) {
    model_mat = glm::translate(glm::rotate(glm::mat4(1.0f), rotation, axis), pos);
    pos_ = pos;
    rotation_ = rotation;
    rotation_axis_ = axis;
    vertices = vertices_;
    indexes = indexes_;
    mesh_buffer = create_mesh_buffer(device, vertices, indexes);

    upload_buffers_(device);
}

Model::Model(SDL_GPUDevice *device, std::string model_path, std::string texture_path, glm::vec3 pos, float rotation,
             glm::vec3 axis) {
    model_mat = glm::translate(glm::rotate(glm::mat4(1.0f), rotation, axis), pos);
    std::tie(mesh_buffer, vertices, indexes) = create_mesh_buffer_from_path(device, model_path);
    std::tie(texture, image_data, texture_transfer_info, texture_transfer_buffer) = create_texture_image(
        device, texture_path);

    upload_buffers_(device);
}

void Model::draw(SDL_GPURenderPass *render_pass, SDL_GPUSampler *sampler, Camera &camera,
                 SDL_GPUCommandBuffer *command_buffer) {
    SDL_GPUBufferBinding vertex_buffer_bindings[1];
    vertex_buffer_bindings[0].buffer = mesh_buffer.vertex_buffer;
    vertex_buffer_bindings[0].offset = 0;

    SDL_GPUBufferBinding index_buffer_bindings[1];
    index_buffer_bindings[0].buffer = mesh_buffer.index_buffer;
    index_buffer_bindings[0].offset = 0;

    SDL_GPUTextureSamplerBinding texture_sampler_bindings[1];
    texture_sampler_bindings[0].texture = texture;
    texture_sampler_bindings[0].sampler = sampler;

    SDL_BindGPUVertexBuffers(render_pass, 0, vertex_buffer_bindings, 1);
    SDL_BindGPUIndexBuffer(render_pass, index_buffer_bindings, SDL_GPU_INDEXELEMENTSIZE_32BIT);
    float lightX = sin(SDL_GetTicks() / 500.0f) * 5.0f;
    float lightZ = cos(SDL_GetTicks() / 500.0f) * 5.0f;
    ModelUniformBuffer uniform_buffer{
        .model = model_mat,
        .view = camera.view,
        .projection = camera.projection,
        .light_pos = {lightX, 1.0f, lightZ}
    };

    SDL_BindGPUFragmentSamplers(render_pass, 0, texture_sampler_bindings, 1);
    SDL_PushGPUVertexUniformData(command_buffer, 0, &uniform_buffer, sizeof(ModelUniformBuffer));
    SDL_DrawGPUIndexedPrimitives(render_pass, indexes.size(), 1, 0, 0, 0);
}

void Model::update_pos(glm::vec3 pos) {
    pos_ = pos;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, pos_);
    trans = glm::rotate(trans, rotation_, rotation_axis_);
    trans = glm::scale(trans, glm::vec3(scale_, scale_, scale_));
    model_mat = trans;
}

void Model::update_rotation(float angle, glm::vec3 axis) {
    rotation_axis_ = axis;
    rotation_ = angle;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, pos_);
    trans = glm::rotate(trans, rotation_, rotation_axis_);
    trans = glm::scale(trans, glm::vec3(scale_, scale_, scale_));
    model_mat = trans;
}

void Model::update_scale(float scale) {
    scale_ = scale;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, pos_);
    trans = glm::rotate(trans, rotation_, rotation_axis_);
    trans = glm::scale(trans, glm::vec3(scale_, scale_, scale_));
    model_mat = trans;
}
