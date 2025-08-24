//
// Created by oliver on 21/08/2025.
//

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "engine_types.h"
#include "camera.h"


class Model {
    float rotation_{};
    glm::vec3 rotation_axis_{};
    glm::vec3 pos_{};

    void upload_buffers_(SDL_GPUDevice *device);

public:
    MeshBuffer mesh_buffer{};
    std::vector<Vertex> vertices;
    std::vector<Uint32> indexes;
    glm::mat4 model_mat{};
    SDL_Surface *image_data;
    SDL_GPUTexture *texture = nullptr;
    SDL_GPUTransferBuffer *texture_transfer_buffer;
    SDL_GPUTextureTransferInfo texture_transfer_info;


    Model(SDL_GPUDevice *device, std::vector<Vertex> &vertices, std::vector<Uint32> &indexes,
          glm::vec3 pos = {0.0f, 0.0f, 0.0f}, float rotation = 0.0f, glm::vec3 axis = {1.0f, 0.0f, 0.0f});

    Model(SDL_GPUDevice *device, std::string model_path, std::string texture_path, glm::vec3 pos = {0.0f, 0.0f, 0.0f},
          float rotation = 0.0f,
          glm::vec3 axis = {1.0f, 0.0f, 0.0f});

    void draw(SDL_GPURenderPass *render_pass, SDL_GPUSampler *sampler, Camera &camera,
              SDL_GPUCommandBuffer *command_buffer);

    void update_pos(glm::vec3 pos);

    void update_rotation(float angle, glm::vec3 axis);
};


#endif //MODEL_H
