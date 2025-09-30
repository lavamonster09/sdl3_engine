//
// Created by oliver on 21/08/2025.
//

#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <tiny_obj_loader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Camera.h"
#include "common.h"


void Model::update_pos(glm::vec3 pos) {
    this->pos = pos;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, pos);
    trans = glm::rotate(trans, rotation, rotation_axis);
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
    model_mat = trans;
}

void Model::update_rotation(float angle, glm::vec3 axis) {
    this->rotation_axis = axis;
    this->rotation = angle;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, pos);
    trans = glm::rotate(trans, rotation, rotation_axis);
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
    model_mat = trans;
}

void Model::update_scale(float scale) {
    this->scale = scale;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, pos);
    trans = glm::rotate(trans, rotation, rotation_axis);
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
    model_mat = trans;
}

void Model::draw(SDL_GPURenderPass *render_pass, SDL_GPUSampler *sampler, const Camera &camera,SDL_GPUCommandBuffer *command_buffer, const std::vector<PointLight *> &lights) {
    SDL_GPUTextureSamplerBinding texture_sampler_bindings[3];
    texture_sampler_bindings[0].texture = texture_diff.texture;
    texture_sampler_bindings[0].sampler = sampler;
    texture_sampler_bindings[1].texture = texture_norm.texture;
    texture_sampler_bindings[1].sampler = sampler;
    texture_sampler_bindings[2].texture = texture_rough.texture;
    texture_sampler_bindings[2].sampler = sampler;
    SDL_BindGPUFragmentSamplers(render_pass, 0, texture_sampler_bindings, 3);
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(render_pass, sampler, camera, command_buffer, lights, model_mat);
    }
}

void Model::load_model(SDL_GPUDevice *device, std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    process_model(device, scene->mRootNode, scene);
}

void Model::process_model(SDL_GPUDevice *device, aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[i];
        meshes.push_back(process_mesh(device, mesh, scene));
    }
}

Mesh Model::process_mesh(SDL_GPUDevice *device, aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<Uint32> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.x = mesh->mVertices[i].x;
        vertex.y = mesh->mVertices[i].y;
        vertex.z = mesh->mVertices[i].z;
        vertex.nx = mesh->mNormals[i].x;
        vertex.ny = mesh->mNormals[i].y;
        vertex.nz = mesh->mNormals[i].z;
        vertex.tx = mesh->mTangents[i].x;
        vertex.ty = mesh->mTangents[i].y;
        vertex.tz = mesh->mTangents[i].z;
        if (mesh->mTextureCoords[0]) {
            vertex.uvx = mesh->mTextureCoords[0][i].x;
            vertex.uvy = mesh->mTextureCoords[0][i].y;
        }
        else {
            vertex.uvx = 0;
            vertex.uvy = 0;
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(device, vertices, indices);
}

void Model::load_textures(SDL_GPUDevice *device, std::string diff_path, std::string norm_path, std::string rough_path) {
    texture_diff = create_texture_image(device, diff_path);
    texture_norm = create_texture_image(device, norm_path);
    texture_rough = create_texture_image(device, rough_path);
}

void Model::upload_texture(SDL_GPUCopyPass *copy_pass, SDL_GPUDevice *device, Texture texture) {
    SDL_GPUTextureRegion texture_buffer_region{};
    texture_buffer_region.texture = texture.texture;
    texture_buffer_region.w = texture.image_data->w;
    texture_buffer_region.h = texture.image_data->h;
    texture_buffer_region.d = 1;

    SDL_UploadToGPUTexture(copy_pass, &texture.texture_transfer_info, &texture_buffer_region, false);
    SDL_DestroySurface(texture.image_data);
    SDL_ReleaseGPUTransferBuffer(device, texture.texture_transfer_buffer);
}

void Model::upload_buffers(SDL_GPUDevice *device) {
    SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(command_buffer);
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].upload_mesh(command_buffer, copy_pass, device);
    }
    upload_texture(copy_pass, device, texture_diff);
    upload_texture(copy_pass, device, texture_norm);
    upload_texture(copy_pass, device, texture_rough);

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(command_buffer);
}

