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
    float rotation_ = 0.0f;
    glm::vec3 rotation_axis_{1.0f, 0.0f, 0.0f};
    glm::vec3 pos_{0.0f, 0.0f, 0.0f};
    float scale_ = 1.0f;

    void upload_buffers_(SDL_GPUDevice *device);

public:
    MeshBuffer mesh_buffer{};
    std::vector<Vertex> vertices;
    std::vector<Uint32> indexes;
    glm::mat4 model_mat{};
    Texture texture{};
    Texture normal_map{};


    Model(SDL_GPUDevice *device, std::vector<Vertex> &vertices, std::vector<Uint32> &indexes,
          glm::vec3 pos = {0.0f, 0.0f, 0.0f}, float rotation = 90.0f, glm::vec3 axis = {0.0f, 0.0f, 0.0f});

    Model(SDL_GPUDevice *device, std::string model_path, std::string texture_path, std::string normal_path, glm::vec3 pos = {0.0f, 0.0f, 0.0f},
          float rotation = 0.0f,
          glm::vec3 axis = {0.0f, 0.0f, 0.0f});

    void draw(SDL_GPURenderPass *render_pass, SDL_GPUSampler *sampler, Camera &camera,
              SDL_GPUCommandBuffer *command_buffer, std::array<PointLight*, 4> &lights);

    void update_pos(glm::vec3 pos);

    void update_rotation(float angle, glm::vec3 axis);

    void update_scale(float scale);

    void draw(SDL_GPURenderPass * render_pass, SDL_GPUSampler * sampler, const Camera & camera, SDL_GPUCommandBuffer * command_buffer, const std::vector<PointLight *> & lights);
};


#endif //MODEL_H
