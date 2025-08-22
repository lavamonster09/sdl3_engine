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
public:
    MeshBuffer mesh_buffer;
    std::vector<Vertex> vertices;
    std::vector<Uint32> indexes;
    glm::mat4 model_mat;

    Model(SDL_GPUDevice *device, std::vector<Vertex> &vertices, std::vector<Uint32> &indexes, glm::mat4 model);

    Model(SDL_GPUDevice *device, char *model_path, glm::mat4 model);

    void draw(SDL_GPURenderPass *render_pass, Camera &camera, SDL_GPUCommandBuffer *command_buffer);
};


#endif //MODEL_H
